#pragma once
#include <string> 
#include <chrono>

using namespace std; 

namespace adaptive_ai 
{
    namespace safety
    {
        struct ResourceLimits
        {
            size_t maxMemoryBytes; 
            chrono :: milliseconds maxCpuTime; 
            size_t maxFileWrites;
            bool networkAccess; 
        };

        struct SandBoxReport
        {
            bool ok;
            string details; 
            double cpuMs;
            size_t memoryUsed; 
        }; 

        class SandboxGuard
        {
            public: 
            // Create a sandbox with resources limits, returns sandbox id.. / 
            string createSandbox(const ResourceLimits &limits); 

            // Execute code or simulation step within sandbox.. // 
            SandBoxReport runInSandbox(const string &sandboxId, const string &entryPoint, const string &inputBlob );

            // Destroy sandbox and free resources.. .// 
            void destroySandbox(const string &sandboxId);
        }; 
    } // namespace safety 
} // namespace adaptive_ai .. 
