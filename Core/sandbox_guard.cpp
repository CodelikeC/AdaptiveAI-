#include "sandbox_guard.h"
using namespace std;

#include <iostream> 
#include <unordered_map>

#include <thread> 
#include <chrono> 

#include <mutex> 
#include <sstream>
#include <random>

using namespace adaptive_ai :: safety; 

namespace 
{
    string generateSandboxId(){
        static const char charset[]
        = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
        static thread_local mt19937 rg{random_device{}()};  
        static thread_local uniform_int_distribution <size_t> pick(0, sizeof(charset) -2); 
        string s; 
        for (int i = 0 ; i< 10  ; i++)
        {
            s += charset[pick(rg)]; 
            return s; 
        }
    }

    class SandboxGuardImpl
    {
        public: 
        struct SandboxContext{
            ResourceLimits limits; 
            chrono :: steady_clock:: time_point startTime; 
            bool active = false; 
        }; 

        unordered_map<string, SandboxContext> sandboxes; 
        mutex mu; 
    }; 

    // ======================================= // 
    static SandboxGuardImpl impl; // dung singleton cuc bo cho demo .../

    string SandboxGuard :: createSandbox(const ResourceLimits &limits)
    {
        lock_guard<mutex> lock(impl.mu);
    string id = generateSandboxId();
    SandboxGuardImpl::SandboxContext ctx;
    ctx.limits = limits;
    ctx.startTime = chrono::steady_clock::now();
    ctx.active = true;

    impl.sandboxes[id] = ctx;
    cout << "[SandboxGuard] Created sandbox " << id
         << " | MaxMem=" << limits.maxMemoryBytes
         << " | MaxCPU(ms)=" << limits.maxCpuTime.count()
         << " | MaxWrites=" << limits.maxFileWrites
         << " | Net=" << (limits.networkAccess ? "ON" : "OFF") << endl;

    return id;
    }

    SandBoxReport SandboxGuard::runInSandbox(
    const string &sandboxId,
    const string &entryPoint,
    const string &inputBlob)
{
    lock_guard<mutex> lock(impl.mu);
    SandBoxReport report;
    report.ok = false;
    report.cpuMs = 0;
    report.memoryUsed = 0;

    auto it = impl.sandboxes.find(sandboxId);
    if (it == impl.sandboxes.end()) {
        report.details = "Sandbox not found";
        cerr << "[SandboxGuard] Error: sandbox not found: " << sandboxId << endl;
        return report;
    }

    auto &ctx = it->second;
    if (!ctx.active) {
        report.details = "Sandbox inactive";
        cerr << "[SandboxGuard] Sandbox inactive: " << sandboxId << endl;
        return report;
    }

    cout << "[SandboxGuard] Running entry '" << entryPoint
         << "' in sandbox " << sandboxId << "..." << endl;

    // --- Giả lập sử dụng CPU ---
    auto start = chrono::steady_clock::now();
    this_thread::sleep_for(chrono::milliseconds(30 + rand() % 50)); // giả CPU workload
    auto end = chrono::steady_clock::now();
    report.cpuMs = chrono::duration<double, milli>(end - start).count();

    // --- Giả lập memory usage ---
    report.memoryUsed = 2000000 + rand() % 3000000; // 2–5 MB giả lập

    // --- Giới hạn kiểm tra ---
    if (report.cpuMs > ctx.limits.maxCpuTime.count()) {
        report.ok = false;
        report.details = "CPU time exceeded";
        cout << "[SandboxGuard] CPU limit exceeded!" << endl;
    } else if (report.memoryUsed > ctx.limits.maxMemoryBytes) {
        report.ok = false;
        report.details = "Memory usage exceeded";
        cout << "[SandboxGuard] Memory limit exceeded!" << endl;
    } else {
        report.ok = true;
        report.details = "Execution OK";
        cout << "[SandboxGuard] Execution successful." << endl;
    }

    return report;
}

void SandboxGuard::destroySandbox(const string &sandboxId) {
    lock_guard<mutex> lock(impl.mu);
    auto it = impl.sandboxes.find(sandboxId);
    if (it != impl.sandboxes.end()) {
        impl.sandboxes.erase(it);
        cout << "[SandboxGuard] Destroyed sandbox " << sandboxId << endl;
    } else {
        cerr << "[SandboxGuard] Attempted to destroy non-existent sandbox "
             << sandboxId << endl;
    }
}

}