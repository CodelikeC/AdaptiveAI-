#include <iostream> 
using namespace std; 
#include <string> 

#include "self_defense.h"

namespace adaptive_ai 
{

    SelfDefense::SelfDefense() {}

    SelfDefense :: ~SelfDefense(){}
    
    void SelfDefense::counter_intrusion(const std::vector<std::string>& threats) {
        for (const auto& threat : threats) 
        {
            isolate_thread(threat);
            log_defense_action("Isolated threat: " + threat);
        }
    }
    
    void SelfDefense::isolate_thread(const std::string& id) {
        std::cout << "[SelfDefense] Isolating suspicious thread or action: " << id << std::endl;
    }
    
    void SelfDefense::log_defense_action(const std::string& action) {
        std::cout << "[SelfDefense] Action logged: " << action << std::endl;
    }
    
    } // namespace adaptive_ai
    