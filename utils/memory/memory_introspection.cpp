#include "memory_introspection.h"
using namespace std; 

#include <sstream>
#include <iomanip>

#if defined(_WIN32)
#include <windows.h>
#include <psapi.h>
#elif defined(__linux__)
     #include <unistd.h>
    #include <fstream>
    #include <sys/sysinfo.h>
#elif defined(__APPLE__)
    #include <sys/types.h>
    #include <sys/sysctl.h>
    #include <mach/mach.h>
#endif

namespace adaptive_ai 
{
    namespace memory
    {
        MemoryIntrospection::MemoryIntrospection() {}

void MemoryIntrospection::set_tracked(std::size_t allocs, std::size_t bytes) {
    std::lock_guard<std::mutex> lk(m_mutex);
    m_tracked_allocs = allocs;
    m_tracked_bytes = bytes;
}

MemorySnapshot MemoryIntrospection::snapshot() const {
    MemorySnapshot snap = collect_process_memory();
    collect_system_memory(snap);

    std::lock_guard<std::mutex> lk(m_mutex);
    snap.tracked_allocs = m_tracked_allocs;
    snap.tracked_bytes = m_tracked_bytes;
    return snap;
}

std::string MemoryIntrospection::report() const {
    auto snap = snapshot();
    std::ostringstream oss;
    oss << "=== Memory Introspection Report ===\n";
    oss << "Process RSS: " << snap.process_rss << " bytes\n";
    oss << "Process Virtual: " << snap.process_virtual << " bytes\n";
    if (snap.total_system > 0) {
        oss << "System RAM: " << snap.total_system << " bytes (Free: " 
            << snap.free_system << ")\n";
    }
    oss << "Tracked Allocations: " << snap.tracked_allocs 
        << " (" << snap.tracked_bytes << " bytes)\n";
    return oss.str();
}

// --- Internal platform-specific implementations ---

MemorySnapshot MemoryIntrospection::collect_process_memory() const {
    MemorySnapshot snap;

#if defined(_WIN32)
    PROCESS_MEMORY_COUNTERS_EX pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) {
        snap.process_rss = pmc.WorkingSetSize;
        snap.process_virtual = pmc.PrivateUsage;
    }
#elif defined(__linux__)
    std::ifstream statm("/proc/self/statm");
    if (statm) {
        long size=0, resident=0;
        statm >> size >> resident;
        long page_size = sysconf(_SC_PAGESIZE);
        snap.process_virtual = size * page_size;
        snap.process_rss = resident * page_size;
    }
#elif defined(__APPLE__)
    mach_task_basic_info info;
    mach_msg_type_number_t count = MACH_TASK_BASIC_INFO_COUNT;
    if (task_info(mach_task_self(), MACH_TASK_BASIC_INFO,
                  reinterpret_cast<task_info_t>(&info), &count) == KERN_SUCCESS) {
        snap.process_rss = info.resident_size;
        snap.process_virtual = info.virtual_size;
    }
#endif

    return snap;
}

void MemoryIntrospection::collect_system_memory(MemorySnapshot& snap) const {
#if defined(_WIN32)
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);
    if (GlobalMemoryStatusEx(&statex)) {
        snap.total_system = statex.ullTotalPhys;
        snap.free_system = statex.ullAvailPhys;
    }
#elif defined(__linux__)
    struct sysinfo info;
    if (sysinfo(&info) == 0) {
        snap.total_system = static_cast<std::size_t>(info.totalram) * info.mem_unit;
        snap.free_system  = static_cast<std::size_t>(info.freeram)  * info.mem_unit;
    }
#elif defined(__APPLE__)
    int mib[2];
    int64_t memsize;
    size_t len = sizeof(memsize);
    mib[0] = CTL_HW;
    mib[1] = HW_MEMSIZE;
    if (sysctl(mib, 2, &memsize, &len, nullptr, 0) == 0) {
        snap.total_system = static_cast<std::size_t>(memsize);
    }

    // free memory
    mach_port_t host_port = mach_host_self();
    vm_size_t page_size;
    mach_port_t object_name;
    mach_msg_type_number_t count;
    vm_statistics64_data_t vmstat;
    count = HOST_VM_INFO64_COUNT;
    if (host_statistics64(host_port, HOST_VM_INFO64, (host_info64_t)&vmstat, &count) == KERN_SUCCESS) {
        if (host_page_size(host_port, &page_size) == KERN_SUCCESS) {
            snap.free_system = static_cast<std::size_t>(vmstat.free_count) * page_size;
        }
    }
#endif
}
    } // namespace memory..
}
