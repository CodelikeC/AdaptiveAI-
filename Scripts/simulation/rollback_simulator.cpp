#include "rollback_simulator.h"
#include "rollback_simulator.h"
using namespace std; 
#include <thread> 
#include <string> 
#include <chrono>
#include <atomic> 

#include <functional> 
#include <random> 

namespace adaptive_ai
{
    namespace simulation
    {
        int RollbackSimulator :: createSnapshot(const string &description)
        {
            Snapshot snap; 
            snap.id = nextId_++; 
            snap.timestamp =  chrono :: system_clock::now(); 
            snap.description = description; 

            snapshots_.push_back(snap); 

            cout <<"RollbackSimulator  - Created snapshot #" + to_string(snap.id) + "-" + description << endl; 
            return snap.id;
        }

        RollBackResult RollbackSimulator :: rollbackTo(int snapshotId)
        {
           auto it = std::find_if(snapshots_.begin(), snapshots_.end(),
                           [snapshotId](const Snapshot& s) { return s.id == snapshotId; });
           if (it == snapshots_.end()){
                cout <<"[RollbackSimulator] - Snapshot ID not found:" + to_string(snapshotId) << endl; 
                return {
                    false, -1, "snapshot not found"
                }; 
           }
           // To do : Thuc te co the restore memory/policy tu snapshot ...// 
           cout <<"RollbackSimulator - Rolling back to snapshot #" + to_string(it -> id) + "-" + it ->description << endl; 
           return {
            true, it -> id , "Rollback successful"
           };                  
        }

        vector <Snapshot> RollbackSimulator :: listSnapshots() const 
        {
            return snapshots_; 
        }

        bool RollbackSimulator :: removeSnapshot(int snapshotId)
        {
            auto it = remove_if(snapshots_.begin(), snapshots_.end(), 
            [snapshotId](const Snapshot &s){
                return s.id == snapshotId; 
            });
            
            if (it != snapshots_.end())
            {
                snapshots_.erase(it, snapshots_.end()); 
                cout <<"Rollback Simulator - Remove snapshot #" + to_string(snapshotId); 
                return true; 
            }
            return false; 
        }

        void RollbackSimulator :: clearAll()
        {
            snapshots_.clear(); 
            cout <<"Rollback Simulator - Clear all snapshots" << endl; 
        }
    }
} // namespace name


