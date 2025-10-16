#pragma once 
#include <functional>

#include <mutex> 
#include <atomic> 

#include <thread>
#include <chrono>

#include <string> 
#include "memory_bridge.h"

#include "sandbox_guard.h"
#include "sync_manager.h"

#include "lifecycle_manager.h"
using namespace std; 

namespace adaptive_ai 
{
    namespace orchestration
    {
        using adaptive_ai :: memory :: MemoryBridge; 
        using adaptive_ai :: memory :: snapshot;
        using adaptive_ai :: safety :: SandboxGuard; 
        using adaptive_ai :: safety :: SandBoxReport;
        using adaptive_ai :: net :: SyncManager; 
        using adaptive_ai :: core :: LifeCycleManager; 
        using adaptive_ai :: core :: LifeCycleState; 

        class RollbackOrchestrator{
            public: 
            RollbackOrchestrator(MemoryBridge *mem, 
            SandboxGuard *guard, 
            SyncManager *sync, 
            LifeCycleManager *lifecycle);
            
            ~RollbackOrchestrator();

            // start orchestrator (register callbacks, start background worker...)..// 
            void start(); 

            // stop orchestrator and background tasks..// 
            void stop(); 

            // manual trigger : evaluate a snapshot by exporting to sandbox and possibly rollback..//
            void evaluateSnapshot(int snapshotId, const string &sandboxId); 

            // Set threshold for rollback score  . Default 0.5 .. // 
            void setRollbackThreshold(double t); 

            private: 
            MemoryBridge *memory_; 
            SandboxGuard *sandbox_; 
            SyncManager *sync_; 
            LifeCycleManager *lifecycle_; 

            mutex mu_; 
            atomic<bool> running_; 
            double rollbackThreshold_; 

            // Internal helper :: computers a rollback score from sandbox report and other signals..
            double computeRollbackScore(const SandBoxReport &report); 

            // Internal orchestration flow for a snapshot..//
            void orchestrate(int snapshotId, const string &sandboxId); 

            // callback when a remote snapshot arrives (from Syncmanager)..
            void onRemoteSnapshot(int snapshotId, const string &payload); 
        }; 
    } // namespace orchestration..
} // namespace adaptive_ai .. 