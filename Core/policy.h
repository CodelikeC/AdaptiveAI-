#pragma once 
#include <iostream> 
#include <vector> 

#include <string> 
using namespace std; 

namespace adaptive_ai 
{
    class Policy
    {
        public: 
        Policy(); 

        // quyet dinh hanh dong dua tren du lieu dau vao .. //
        string decide_action(const vector<double> &state);
        // cap nhat poilcy theo tin hieu tu meta-learning hoac evolution.. // 
        void update_policy(const vector<double> &new_weights);
        
        private: 
        vector<double> weights; 
        // cac tham so quyet dinh 

        double compute_score(const vector<double> &state) const ; 
    };
}