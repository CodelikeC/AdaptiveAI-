#pragma once 
#include <string> 
#include <functional>
using namespace std; 

namespace adaptive_ai 
{
    namespace net 
    {
        enum class SyncNode
        {
            PUSH, PULL, PEER
        }; 

        class SyncManager
        {
            public: 
            SyncManager(); 
            ~SyncManager();
            // start network sync service (bind, advertise)...
            bool start(const string &bindAddr); 

            // stop servive .. //
            void stop(); 

            // Push snapshot to remote work (async).. 
            bool pushSnapshot(const string &nodeaAddr, int snapshotId); 

            // Request snapshot from node.. // 
            bool requestSnapshot(const string &nodeAddr, int snapshotId);
            
            // Register callback when remote snapshot arrives .. // 
            void onSnapshotReceive(const function<void(int snapshotId, const string &payload)> &cb);
        }; 
    } // namespace net ... 
} // namespace adaptive_ai ...