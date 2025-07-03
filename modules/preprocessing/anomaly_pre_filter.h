#ifndef anomaly_pre_filter_h 
#define anomaly_pre_filter_h 

#include <iostream> 
#include <string>
#include <vector> 
#include <regex> 
using namespace std; 

class AnomalyFilter
{
    public: 
    AnomalyFilter(); 
    ~AnomalyFilter(); 

    vector<string> filterSuspicious(const std::vector<std::string>& rawData);

    private: 
    bool isSuspicious(const string &line); 
};

#endif 