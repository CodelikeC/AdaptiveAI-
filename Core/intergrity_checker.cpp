#include "intergrity_checker.h"

#include <fstream> 
#include <sstream> 
#include <iomanip>

#include <filesystem> 
#include <mutex> 
#include "../utils/logger.h"

namespace adaptive_ai 
{
    namespace core
    {
        static string to_hex_string(size_t v)
        {
            ostringstream oss;
            oss << hex << v;
            return oss.str();
        }
        IntergrityReport IntergrityChecker :: validateFile(const string &path)
        {
            IntergrityReport rep ; 
            rep.ok =  false; 

            try 
            {
                if (!filesystem:: exists(path))
                {
                    rep.message = "file not found" + path; 
                    rep.failedItems.push_back(path); 
                    cout <<"Intergrity Checker :"  + rep.message << endl; 
                    return rep;
                }

                // Compute hash (placeholder). Replace computeSha256 with real SHA256 in production.... 
                string hash = computeSha256(path); 

                // Optionally we could compare to a manifest, but this method just reports the computed 
                rep.ok = true;
                rep.message = "File exists. Computed - hash:" + hash; 
                cout <<"Intergrity Checker - ValidateFile success:" + path + "hash = " + hash;
                return rep; 
            }catch(exception &ex){
                cout << endl; 
                rep.ok = false;
                rep.message = string("Exception validating file:") + ex.what(); 
                rep.failedItems.push_back(path); 
                cout <<"Intergrity Checker - Exception:" + rep.message << endl; 
                return rep; 
            }
        }

        IntergrityReport IntergrityChecker :: validateAll(const vector<string> &path)
        {
            IntergrityReport summary; 
            summary.ok = true; 
            summary.message = "All checked"; 

            for (const auto &p : path)
            {
                IntergrityReport r = validateFile(p); 
                if (!r.ok)
                {
                    summary.ok = false; 
                    summary.failedItems.push_back(p); 
                }
            }
            if (!summary.failedItems.empty())
            {
                summary.message = "Some files failed intergrity check"; 
                cout << "Intergrity Checker - Validate ALL : Failures" + to_string(summary.failedItems.size()) << endl; 
            }
            else 
            {
                cout <<"Intergrity Checker  - validate all: all good" + to_string(path.size()) + "files)" << endl;  
            }
            return summary ; 
        }

        string IntergrityChecker :: computeSha256(const string &path)
        {
            try 
            {
                ifstream ifs(path, ios :: binary); 
                if (!ifs)
                {
                    cout <<"Intergrity Checker - computeSha256 : cannot open file" + path << endl; 
                    return ""; 
                }
                ostringstream oss; 
                oss <<ifs.rdbuf();
                string contents  = oss.str(); 

                // use std :: hash as a cheap placeholder .. /////
                size_t h = hash<string> {}(contents); 
                return to_hex_string(h); 
            } catch(exception &ex)
            {
                cout << string("Intergrity Checker") + ex.what() << endl; 
                return ""; 
            }
        }

        void IntergrityChecker :: loadTrustStore(const string &trustStorePath)
        {
            try {
        if (!filesystem::exists(trustStorePath)) {
            Logger::warn("[IntegrityChecker] loadTrustStore: not found " + trustStorePath);
            return;
        }
        ifstream ifs(trustStorePath);
        string content((std::istreambuf_iterator<char>(ifs)), istreambuf_iterator<char>());
        // For now we don't parse; in production this should populate a set of trusted keys/hashes/manifests.
        Logger::info("[IntegrityChecker] loadTrustStore: loaded trust store (" + std::to_string(content.size()) + " bytes)");
    } catch (const std::exception& ex) {
        Logger::error(std::string("[IntegrityChecker] loadTrustStore exception: ") + ex.what());
    }
        }
    }
}