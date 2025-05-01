#include <iostream> 
using namespace std ; 
#include <vector> 
#include <string> 

#include "system_monitor.h"
#include "utils/logger.h"

using namespace adaptive_ai;
struct process_info;

system_monitor :: system_monitor(){
    cout << endl; 
    Logger :: info("System Monitor"); 

}
system_monitor :: ~system_monitor()
{
    cout << endl; 
    Logger :: info("End process"); 
}

void system_monitor :: record_process(const process_info &process)
{

    if (process_log.size() >= max_log_size)
    {
        process_log.erase(process_log.begin()); // remove oldest
    }
    process_log.push_back(process);

    Logger::debug("Recorded process: ", process.name, " [PID=", process.pid, "] CPU=", process.cpu_usage, "% MEM=", process.memory_usage, "MB");
}

vector<process_info> get_high_usage_process(double threshold)
{
    vector<process_info> high_usage;
    vector<process_info> process_log; 
    for (const auto &proc : process_log)
    {
        if (proc.cpu_usage > threshold)
        {
            high_usage.push_back(proc);
            Logger::warn("High CPU usage detected: ", proc.name, " (", proc.cpu_usage, "%)");
        }
    }
    return high_usage;
}

map <string, int> summarize_process_name()
{
    vector<process_info> process_log;
    map<string, int> summary;
    for (const auto &proc : process_log)
    {
        summary[proc.name]++;
    }

    Logger::info("Process summary generated.");
    return summary;
}
