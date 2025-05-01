#include <iostream> 
using namespace std; 
#include <string> 

#include <algorithm> 
#include <vector> 

namespace adaptive_ai
{
    class SelfDefense
    {
        public:
        SelfDefense(); 
        ~SelfDefense();

        // kich hoat co che tu phan ung .. // 
        void counter_intrusion(const vector<string>&threats);
        
        protected: 
        void isolate_thread(const std::string& id);
        void log_defense_action(const std::string& action);
    }; 
}