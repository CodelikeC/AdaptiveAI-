#include "rollback_orchestrator.h"

#include <iostream>
#include <sstream>

using namespace std;
using namespace adaptive_ai::orchestration;

RollbackOrchestrator ::RollbackOrchestrator(
    MemoryBridge *mem, 
            SandboxGuard *guard, 
            SyncManager *sync, 
            LifeCycleManager *lifecycle
) : memory_(mem), sandbox_(guard), sync_(sync), lifecycle_(lifecycle),
      running_(false), rollbackThreshold_(0.5) {}
      
RollbackOrchestrator :: ~RollbackOrchestrator()
{
    stop(); 
}

void RollbackOrchestrator ::start()
{
    lock_guard<mutex> lock(mu_); 
    if (running_) return; 
    running_ = true; 

    // Register callback with SyncManager to receive remote snapshots..// 
    if (sync_)
    {
        sync_ -> onSnapshotReceive([this] (int snapshotId, const string &payload){
            this -> onRemoteSnapshot(snapshotId, payload); 
        });
        cout <<"RollbackOrchestrator - Registered snapshot receive callback with Syncmanager:" << endl;  
    }
    cout <<"RollbackManager - started" << endl; 
}

void RollbackOrchestrator:: stop()
{
    lock_guard<mutex> lock(mu_); 
    if (!running_) return ; 
    running_ = false; 
    cout <<"RollbackOrchestrator - Stopped" << endl; 
}

void RollbackOrchestrator :: setRollbackThreshold(double t)
{
    if (t <0.0) t = 0.0; 
    if (t > 0.0) t = 1.0; 
    rollbackThreshold_ = t; 
}

double RollbackOrchestrator :: computeRollbackScore(const SandBoxReport &report)
{
    // Simple heuristic;
    // If solution ok => high score = 1.0 
    // Else score decreases with CPU and memory exceedance (normalize)..// 
    if (report.ok) return 1.0; 
    if (!report.ok) return 0.0; 

    // Penalize by CPU and memory usage callback to some assumed maxima.. // 
    double cpuPenalty = min (1.0, report.cpuMs /1000.0); 
    // assume 1000 ms is terrible.. .// 
    double memPenalty = min(1.0, static_cast<double> (report.memoryUsed) / (50 *1024 *1024)); 
    double base = 0.2 ; // baseline for failed execs (we do not want to accept blindly)..// 

    double score = max(0.0, 1.0 - (cpuPenalty *0.6 + memPenalty *0.4)); 
    score = min(1.0, max(0.0, score)); 

    // Combine with base .. //
    score = (score + base) / 2.0; 
    return score; 
}

void RollbackOrchestrator :: orchestrate(int snapshotId, const string &sandboxId){
    if (!running_){
        cout << "Rollback Orchestrator - Not running; abort orchestrate" << endl; 
        return ; 
    }
    cout <<"[RollbackOrchestrator] Orchestrating snapshot #" << snapshotId 
    <<"into sandbox" << sandboxId << endl; 

    // 1 ) Export snapshot to sandbox..// 
    bool exported = false;
    if (memory_){
        exported = memory_ -> exportSnapshotToSandbox(snapshotId, sandboxId); 
    }
    else {
        cerr <<"Rollback Orchestrator - No Memory Bridge available" << endl; 
    }
    if (!exported){
        cerr <<"RollbackOrchestrator - Export failed for snapshot" << snapshotId << endl;
        // Consider immediate rollback attempt or alert, here we log and return..
        return; 
    }

    // 2 ) Run in sandbox and get report ..// 
    SandBoxReport report = sandbox_ -> runInSandbox(sandboxId, "Entry_orchestrator", "{}"); 

    // 3) Compute rollback score.. // 
    double score = computeRollbackScore(report); 

    cout <<"Rollback Orchestrator - Sandbox report for snapshot #" << snapshotId 
    <<"=> ok=" << (report.ok? "true" : "false") 
    <<"CpuM = " << report.cpuMs 
    << "mem = " << report.memoryUsed 
    <<"score =" << score << endl; 

    // 4) based on score, decide merge or rollback.. // 
    if (score >= rollbackThreshold_){
        cout <<"Rollback Orchestrator - snapshot #" << snapshotId 
        <<"Accepted (score >= thresholdId)" << endl; 

        // if accepted, merge sandbox result back..
        if (memory_){
            memory_ -> mergeSandboxResult(snapshotId, "sandbox_out_ok", score); 
        }
        // optionally push snapshot to peers,..//
        if (sync_){
            // push to a default peer set , for demo we skip or call push..
            sync_ -> pushSnapshot("NodePear", snapshotId); // nodepeer should be replaced..//
        }
        else {
            cout <<"RollbackOrchestrator" << snapshotId << "rejected (score < threshold)" << endl; 
            // Trigger orchestrated rollback via lifecycle manager..
            if (lifecycle_)
            {
                bool trig = lifecycle_ -> triggerRollback(snapshotId); 
                cout <<"RollbackOrchestrator - lifecycle" << (trig ? "true" : "false"); 
            }
            // attempt to restore snapshot explicity ..//
            if (memory_){
                bool restored = memory_-> restoreSnapshot(snapshotId);
                cout <<"RollbackOrchestrator - memory restoreSnapshot returned:" << (restored ? "true" : "false");
            }
        }
    }

}

void RollbackOrchestrator ::evaluateSnapshot(int snapshotId, const string &sandboxId){
    // Public API to schedule orchestration asynchronorously..
    thread([this, snapshotId , sandboxId](){
        this -> orchestrate(snapshotId, sandboxId); 
    }).detach();

}

void RollbackOrchestrator :: onRemoteSnapshot(int snapshotId, const string &payload)
{
    // Called whenever SyncManager pushes/pulls a snapshot..// 
    cout <<"RollbackOrchestrator - on RemoteSnapshot : id = " << snapshotId
    <<"Payload(len) = " << payload.size() << endl;

    // Create or reuse a sandbox id (simple scheme: sandbox_<snapshotID>) 
    ostringstream oss; 
    oss <<"Sandbox_" << snapshotId ; 
    string sandboxID = oss.str() ; 

    // For safety, create sandbox with conservative limits(example).. // 
    adaptive_ai :: safety :: ResourceLimits limits;
    limits.maxMemoryBytes = 16 *1024*1024; 
    limits.maxCpuTime = chrono::milliseconds(500);
    limits.maxFileWrites = 5;
    limits.networkAccess = false;

    // create sandbox
    std::string createdId = sandbox_->createSandbox(limits);
    if (createdId.empty()) {
        cerr << "[RollbackOrchestrator] Failed to create sandbox for remote snapshot." << endl;
        return;
    }

    // Optionally persist payload into memory as a new snapshot (or use existing snapshotId)
    // For demo, we trust the incoming snapshotId exists in MemoryBridge; orchestrate using it.
    // Run evaluation asynchronously
    evaluateSnapshot(snapshotId, createdId);

    // After some time, we can destroy sandbox (or let orchestrate do it). For safety destroy after delay.
    std::thread([this, createdId]() {
        std::this_thread::sleep_for(std::chrono::seconds(3));
        sandbox_->destroySandbox(createdId);
    }).detach();
}
