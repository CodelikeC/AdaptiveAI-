#include <iostream> 
using namespace std; 
#include "data_Cleaner.h"

#include <algorithm> 

#include <regex> 
#include <cmath> 
#include "utils/logger.h"

#include <sstream> 
#include <cctype> 

namespace preprocessing 
{
    vector<double> DataCleaner :: clean_numeric_data(const vector<double> &data)
    {
        vector<double> cleaned; 
        for (double val : data)
        {   
            if (!isnan(val) and !isinf(val))
            {
                cleaned.push_back(val);
            }
            else{
                adaptive_ai :: Logger :: warn("DataCleaner:Skipped invalid numeric value."); 

            }
        }
        return cleaned;
    }
    unordered_map <string, string> DataCleaner :: clean_key_value_data
    (const unordered_map<string, string> &raw_data)
    {
        unordered_map<string, string> cleaned;
        for (const auto &kv : raw_data)
        {
            const string &key = kv.first; 
            const string &value = kv.second;

            if (!is_invalid_string(key) and !is_invalid_string(value))
            {
                cleaned[key] = value; 
            }
            else{
                adaptive_ai :: Logger :: warn("Data Cleaner: Removed field due to interaction");
            }
        }
    }

    bool DataCleaner :: is_invalid_string(const string &str)
    {
        string trimmed = trim(str); 

        if (trimmed.empty())
        return true; 
        static const regex nonsense_pattern("^(null|none|undefined|n/a|\\s*)$", regex_constants :: icase);
        if (regex_match(trimmed, nonsense_pattern))
        return true;
        
        if (trimmed.length() > 256 or trimmed.find_first_not_of("\t\n\r\f\v") )
        return true; 

        return false; 

    }

    string DataCleaner :: trim(const string &str)
    {
        size_t start = str.find_first_not_of("\t\r\n");
        if (start == string :: npos)
        return ""; 

        size_t end = str.find_last_not_of("\t\r\n"); 
        return str.substr(start, end  - start + 1);
    }

} // namespace preprocessing ..// 