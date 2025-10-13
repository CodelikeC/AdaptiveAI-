#pragma once 
#include <string> 
#include <vector> 
using namespace std; 

namespace adaptive_ai 
{
    namespace core 
    {
        struct IntergrityReport
        {
            bool ok;
            string message; 
            vector<string> failedItems; 
        }; 

        class IntergrityChecker
        {
            public: 
            // Validate single file (binary/ plugin path)
            IntergrityReport validateFile(const string &path); 

            // Validate collection(core modules, plugins)..// 
            IntergrityReport validateAll(const vector<string> &path); 

            // Optionally compute and return hash (sha256) for a file .. // 
            string computeSha256(const string &path); 

            // Set trusted signer public key, manifest, etc...
            void loadTrustStore(const string &trustScorePath); 
        }; 
    } // namespace core .. 
} // namespace adaptive_ai ...