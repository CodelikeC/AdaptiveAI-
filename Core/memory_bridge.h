#pragma once 
#include <string> 
#include <optional> 
#include <vector> 
#include <map> 
#include <mutex> 

using namespace std; 

namespace adaptive_ai 
{
    namespace memory
    {
        struct snapshot
        {
            int id; 
            string serializedState; 
            string metadata; 
            long timestamp; 
        }; 

        class MemoryBridge
        {
            public: 
            // Create snapshot and persist (returns snapshot id)
            int createSnapshot(const string &description); 

            // Restore snapshot into live state(simulation or real).. // 
            bool restoreSnapshot(int snapshotId); 

            // Export snapshot to sandbox (simulate with this state); 
            bool exportSnapshotToSandbox(int snapshotId, const string &sandboxId); 

            // Merge sandbox results back into memory (with policy).. 
            bool mergeSandboxResult(int snapshotId, const string &sandboxResult, double confidence); 

            // list snapshots.. 
            vector<snapshot> listSnapshot() const;

            private:
            map<int, snapshot> snapshots_; // Luu tru tat ca snapshot theo id.. 
            mutable mutex mu_; // Bao ve do luong
            int nextId_; // sinh id tang dan

            // Sinh timestamp hien tai 
            long currentTimeStamp() const; 
        };

    } // namespace memory ..
} // namespace adaptive_ai //