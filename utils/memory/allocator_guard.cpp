#include "allocator_guard.h"
#include <new>

#include <cstdlib> 
#include <sstream> 

namespace adaptive_ai {
namespace memory {

AllocatorGuard::AllocatorGuard() {}

AllocatorGuard::~AllocatorGuard() {
    rollback();
}

void* AllocatorGuard::allocate(std::size_t size, const char* file, int line) {
    void* ptr = std::malloc(size);
    if (!ptr) return nullptr;
    std::lock_guard<std::mutex> lk(m_mutex);
    m_map[ptr] = AllocationInfo{size, file, line};
    m_total_allocs++;
    m_active_bytes += size;
    return ptr;
}

void AllocatorGuard::deallocate(void* ptr) {
    if (!ptr) return;
    std::lock_guard<std::mutex> lk(m_mutex);
    auto it = m_map.find(ptr);
    if (it != m_map.end()) {
        m_active_bytes -= it->second.size;
        m_map.erase(it);
    }
    std::free(ptr);
}

void AllocatorGuard::rollback() {
    std::lock_guard<std::mutex> lk(m_mutex);
    for (auto& kv : m_map) {
        std::free(kv.first);
    }
    m_map.clear();
    m_active_bytes = 0;
}

std::size_t AllocatorGuard::active_allocations() const {
    std::lock_guard<std::mutex> lk(m_mutex);
    return m_map.size();
}

std::size_t AllocatorGuard::active_bytes() const {
    std::lock_guard<std::mutex> lk(m_mutex);
    return m_active_bytes;
}

std::size_t AllocatorGuard::total_allocations() const {
    std::lock_guard<std::mutex> lk(m_mutex);
    return m_total_allocs;
}

std::string AllocatorGuard::dump_leaks() const {
    std::lock_guard<std::mutex> lk(m_mutex);
    if (m_map.empty()) return "No leaks detected.";
    std::ostringstream oss;
    oss << "Leaks detected (" << m_map.size() << " allocations):\n";
    for (const auto& kv : m_map) {
        oss << " - ptr=" << kv.first
            << " size=" << kv.second.size;
        if (kv.second.file) {
            oss << " at " << kv.second.file << ":" << kv.second.line;
        }
        oss << "\n";
    }
    return oss.str();
}

} // namespace memory
} // namespace adaptive_a

