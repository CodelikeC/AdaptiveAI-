#pragma once 
#include <iostream> 
#include <vector> 
#include <chrono>
#include <string> 
#include <algorithm> 
using namespace std; 

namespace adaptive_ai{
    namespace simulation{
        struct Snapshot
        {
            int id; 
            chrono :: system_clock::time_point timestamp;
            string description; 
            // Todo : co the mo rong memory dump, policy state,  metrics,... 
        }; 
        // ket qua cua mot lan rollback thu nghiem.. 
        struct RollBackResult
        {
            bool success; 
            int rolledBackToId;
            string message; 
        }; 

        class RollbackSimulator
        {

            private:
            vector<Snapshot> snapshots_;
            int nextId_ = 1;
            
            public: 
            // Tao mot snapshot moi voi mo ta...//
            int createSnapshot(const string &description); 

            // THuc hien rollback va snapshot ID da luu.. // 
            RollBackResult rollbackTo(int snapshotId);

            // tra ve danh sach snapshot hien co .. // 
            vector<Snapshot> listSnapshots() const; 

            // Xoa snapshot (vi du khi da update )
            bool removeSnapshot(int snapshotId); 

            // Xoa toan bo snapshot..// 
            void clearAll(); 
        }; 
    }
}
