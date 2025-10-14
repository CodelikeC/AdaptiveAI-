#include "memory_bridge.h"
using namespace std;

#include <iostream>
#include <string> 

#include <vector> 
#include <functional>

#include <sstream>
using namespace adaptive_ai :: memory; 

MemoryBridge :: MemoryBridge() : nextId_(1){}

long MemoryBridge :: currentTimeStamp() const 
{
    using namespace std :: chrono; 
    return duration_cast<milliseconds>(
        system_clock::now().time_since_epoch()
    ).count(); 
}

int MemoryBridge :: createSnapshot(const string &description)
{
    lock_guard<mutex>lock(mu_); 
    snapshot snap;
    snap.id = nextId_++; 
    snap.serializedState = "SystemState" + to_string(snap.id); // Gia lap..// 
    snap.metadata = description; 
    snap.timestamp = currentTimeStamp(); 
    
    snapshots_[snap.id] = snap; 
    cout <<"MemoryBridge - Created snapshot #"
    << snap.id << ")" << description << endl;
    return snap.id; 
}

bool MemoryBridge :: restoreSnapshot(int snapshotId)
{
    lock_guard<mutex> lock(mu_); 
    auto it = snapshots_.find(snapshotId); 
    if (it == snapshots_.end())
    {
        cerr <<"Memory Bridge: Snapshot not found" << snapshotId << endl; 
        return false; 
    }

    cout <<"MemoryBridge - Restoring snapshot #" 
    << snapshotId << "->" << it -> second.serializedState << endl; 

    // Todo : Deserialize into live memory if implemented...
    return true; 
}

bool MemoryBridge :: exportSnapshotToSandbox(int snapshotId, const string &sandboxId)
{
    lock_guard<mutex> lock(mu_); 
    auto it = snapshots_.find(snapshotId); 
    if (it == snapshots_.end()){
        cerr <<"[Memory Bridge] Snapshot not found for sandbox report" << snapshotId << endl; 
        return false; 
    }

    cout <<"Memorybridge - Exporting snapshot #" << snapshotId
    << "To sandbox :" << sandboxId << endl ;

    return true;
}

bool MemoryBridge :: mergeSandboxResult(int snapshotId, const string &sandboxResult, double confidence)
{
    lock_guard<mutex> lock(mu_); 
    auto it = snapshots_.find(snapshotId); 
    if (it == snapshots_.end())
    {
        cerr <<"Memory Bridge Cannot merge, snapshot not found:" << snapshotId << endl; 
        return false; 
    }

    cout <<"MemoryBridge - merging result from sandbox into snapshot #" 
    << snapshotId << "(Confidence) :" << confidence << ")" << endl ; 

    // Gia lap merge logic : chi ghi nhan neu confidence ..// 
    if (confidence > 0.6){
        it -> second.serializedState += "+ merged" + sandboxResult +"]"; 
        cout <<"-> Merge accepted." << endl; 
    }else {
        cout <<"-> Merge discarted (low confidence)" << endl;
    }
    return true; 
}

vector<snapshot> MemoryBridge:: listSnapshot() const 
{
    lock_guard<mutex> lock(mu_);
    vector<snapshot> list;
    for (const auto &[id, snap] : snapshots_)
        list.push_back(snap);
    return list;
}
