#ifndef data_labeler_h 
#define data_labeler_h 

#include <iostream>
#include <string>
using namespace std; 

#include <vector> 
#include <unordered_map>

enum class Label
{
    NORMAL, 
    ANOMALY,
    SUSPICIOUS,
    UNKNOWN
}; 

class DataLabeler
{
    public:
    DataLabeler(); 
    ~DataLabeler(); 

    // main API ..// 
    Label labelData(const string &input); 
    string labelToString(Label label); 

    // rule based config ..//
    void loadKeywordRules(); 
    void addKeywordRule(const string &keyword, Label label); 

    private: 
    unordered_map<string, Label> keyword_rules;
}; 
#endif 