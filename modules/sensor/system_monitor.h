#include <iostream>
using namespace std;
#include <map> 
#include <string> 
#include <vector> 

struct process_info
{
    int pid; 
    string name; 
    double cpu_usage; 
    double memory_usage; 
    long timestamp; 
};

namespace adaptive_ai 
{
    class system_monitor
    {
        public: 
        system_monitor(); 
        ~system_monitor();  

        void record_process(const process_info &process);
        vector<process_info>get_high_usage_processes(double cpu_threshold);
        map<string, int> summarize_process_name() const; 
        
        private: 
        vector<process_info> process_log; 
        size_t max_log_size = 100; // kich thuoc cua log 
    }; 
}