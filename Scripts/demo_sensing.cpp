#include <iostream> 
using namespace std; 

#include <vector> 
#include <string> 

#include <unordered_map>

#include "../modules/preprocessing/data_Cleaner.h"
#include "../modules/preprocessing/feature_extraction.h"

#include "../utils/logger.h"

using namespace adaptive_ai;  
using namespace preprocessing;

using namespace feature;

int main ()
{
    Logger :: info("=== Demo : Human-AI sensing and processing ===");

    // tao du lieu mo phong tu nguoi dung .. // 
    vector<std::unordered_map<std::string, std::string>> user_sessions = {
        {
            {"user_id", "alice_01"},
            {"emotion", "happy"},
            {"activity", "study"},
            {"time_spent", "95"},
            {"location", "home"},
        },
        {
            {"user_id", "bob_22"},
            {"emotion", "tired"},
            {"activity", "gaming"},
            {"time_spent", "140"},
            {"location", "cafe"},
        },
        {
            {"user_id", "charlie_99"},
            {"emotion", "neutral"},
            {"activity", "reading"},
            {"time_spent", "80"},
            {"location", "library"},
        }
    };
     // clean + feature extraction cho tung phien .. 
    DataCleaner cleaner;
    FeatureExtractor extractor; 

    int index = 1 ; 
    for (const auto &raw_data : user_sessions)
    {
        Logger :: info("Session", index, ":Raw Input");
        for (const auto &[k , v]: raw_data)
        {
            cout <<"-" << k << "=" << v << endl; 
        }

        auto cleaned = cleaner.clean(raw_data); 
        auto features = extractor.extract_features(cleaned); 
        // double features; 
        cout << "=> Feature vector:" << endl;
        for (double f : features)
        cout <<f << ""; 
        cout << endl;
        ++ index; 
    }

    Logger :: info ("== Demo sensing Completes");
    return 0; 
}
