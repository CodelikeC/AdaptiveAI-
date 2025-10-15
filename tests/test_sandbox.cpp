#include <iostream> 
using namespace std; 
#include "../core/sandbox_guard.h"
#include "../core/sandbox_guard.cpp"

using namespace adaptive_ai :: safety; 

int main ()
{
    SandboxGuard guard; 
    ResourceLimits limits{
         4 * 1024 * 1024, 
        chrono::milliseconds(100), 10, false
    };

    auto id = guard.createSandbox(limits);
    auto report = guard.runInSandbox(id, "demo_function", "{}");

    cout <<"Result :" << report.details
    <<" | CPU :" << report.cpuMs 
    <<"ms | Mem:" << report.memoryUsed << "bytes\n"; 

    guard.destroySandbox(id); 
    return 0; 
}