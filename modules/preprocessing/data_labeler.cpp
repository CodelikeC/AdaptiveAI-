#include "data_labeler.h"
using namespace std;
#include <windows.h> 
#include <algorithm>
#include <iostream> 

DataLabeler :: DataLabeler()
{
    loadKeywordRules(); 
}

DataLabeler :: ~DataLabeler()
{
    // Data Labeler ..  
}

void DataLabeler :: loadKeywordRules()
{
    // Cau hinh ban dau cac keyword de gan nhan .. 
    keyword_rules["failed"] = Label :: ANOMALY; 
    keyword_rules["unauthorized"] = Label :: SUSPICIOUS ;
    keyword_rules["error"]  = Label :: ANOMALY; 
    keyword_rules["login"] = Label :: NORMAL ;
    keyword_rules["ping"] = Label :: NORMAL ;
}

void DataLabeler :: addKeywordRule(const string &keyword, Label label )
{
    keyword_rules[keyword] = label;
}

Label DataLabeler :: labelData(const string &input)
{
    for (const auto&[keyword, label] : keyword_rules)
    {
        if (input.find(keyword) != string ::npos)
        {
            return label; 
        }
    }
    return Label :: UNKNOWN;
}

string DataLabeler :: labelToString(Label label)
{
    switch(label)
    {
        case Label :: NORMAL : return "Normal"; 
        case Label :: ANOMALY: return "ANOMALY"; 
        case Label :: SUSPICIOUS: return "UNKNOWN";
        case Label :: UNKNOWN : return "UNKNOWN"; 
        default: return "UNKNOWN"; 
    }
}