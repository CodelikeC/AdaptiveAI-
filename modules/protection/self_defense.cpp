#include <iostream> 
using namespace std; 

#include "self_defense.h"

namespace adaptive_ai 
{

    SelfDefense::SelfDefense() {}

    SelfDefense :: ~SelfDefense(){
        cout << endl; 
    }
    
    vector <string> SelfDefense :: conunter_instrusion(const vector<string> &threats)
    {
        vector<string> actions; 
        for (const auto &threat : threats)
        {
            isolate_thread(threat); 
            string action = "Isolated threat :" + threat; 
            log_defense_action(action); 
            actions.push_back(action); 

        }
        return actions ;
    }

    void SelfDefense :: isolate_thread(const string &id)
    {
        if (logger_)
        logger_("INFO", "[SelfDefense]" +id); 
    }

    void SelfDefense :: log_defense_action(const string &action)
    {
        if (logger_)
        logger_("DEBUG", "SelfDefense" + action); 
    }
    
} // namespace adaptive_ai

    
