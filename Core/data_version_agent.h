#ifndef DATA_VERSION_AGENT_H 
#define DATA_VERSION_AGENT_H 

#include <string> 
#include <map> 
#include <unordered_map>
#include <vector> 

#include <filesystem> 
#include <chrono> 

#include "../utils/logger.h"
// #include "../utils/metrics.h"

using namespace std; 

namespace AdaptiveAI 
{
    struct DataHashRecord
    {
        string path; 
        string hash; 
        uintmax_t size; 
        time_t timestamp; 
    }; 

    class DataVersionAgent
    {
        private: 
        string basePath; 
        string metaPath; 
        string versioningPath; 

        unordered_map<string, DataHashRecord> lastSnapshot; 
        double driftThreshold = 0.12; // do lech du lieu cho phep 
        bool autoSnapshot = true; 
        
        string computeFileHash(const string &path); 
        void saveMetaHash(const unordered_map<string, DataHashRecord> &snapshot);
        bool detectDrift(const unordered_map<string, DataHashRecord> &newsnapshot); 
        void createSnapshot(); 
        void monitorCycle(); 
        
        public: 
        DataVersionAgent(const string &base = "data/"); 
        void intialize(); 
        void forceSnapshot(); 
        void setAutoSnapshot(bool enabled){
            autoSnapshot = enabled;
        }
    }; 
}

#endif // DATA_version_AGENT_H 