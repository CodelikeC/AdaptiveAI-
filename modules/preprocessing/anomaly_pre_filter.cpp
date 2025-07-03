#include <iostream>
using namespace std;
#include "anomaly_pre_filter.h"

AnomalyFilter :: AnomalyFilter(){}
AnomalyFilter :: ~AnomalyFilter(){}

vector<string> AnomalyFilter ::  filterSuspicious(const std::vector<std::string>& rawData)
{
    vector<string> filtered; 
    for (const auto &line : rawData)
    {
        if (isSuspicious(line))
        {
            filtered.push_back(line); 
        }
    }

    return filtered; 
}

bool AnomalyFilter :: isSuspicious(const string &line)
{
    regex pattern("(error|fail|unauthor)"); 
    return regex_search(line, pattern) ;
}