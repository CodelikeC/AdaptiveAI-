#pragma once 
#include <iostream> 
using namespace std; 
#include <string> 

#include <algorithm> 
#include <vector> 
#include <functional>

namespace adaptive_ai 
{
    class SelfDefense
    {
        public:
        // optional logger callback : logger (level, msg)
        using Logger = std:: function<void(const std:: string &lable, const std:: string &msg)>;
        explicit SelfDefense (Logger logger = nullptr); 

        ~SelfDefense();  

        // kich hoat co che doi pho, tra ve list hanh dong thuc hien ,,,
        std :: vector< std :: string> conunter_instrusion(const std :: vector<std:: string> &threats); 

        private: 
        void isolate_thread(const std :: string &id); 
        void log_defense_action(const std:: string &action); 

        Logger logger_; 

    };
} // namespace adaptive_ai .. // 

