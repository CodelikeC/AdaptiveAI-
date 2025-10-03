#include "memory_snapshot.h"
using namespace std; 
#include <map> 
#include <algorithm> 
#include <cstring> 
#include <cstdint> 

namespace adaptive_ai 
{
    namespace memory
    {
        string MemorySnapshot :: to_string(bool pretty)
        {
            ostringstream oss;
            auto t_c = chrono :: system_clock::to_time_t(timestamp); 
            if (pretty)
            {
                oss <<"\n";
                oss <<"==== Memory Snapshot=====" << endl;
                oss << "Timestamp:" << put_time(localtime(&t_c), "%F %T") << endl; 
                oss <<"Process RSS:" << process_rss <<"bytes" << endl; 
                oss <<"Process Virtual:" << process_virtual << "Bytes\n";
                oss <<"System Ram:" << total_system << "Bytes()" << endl; 
                oss <<"System Free:" << free_system << "Bytes" << endl; 
                oss <<"Tracked allocations:" << tracked_allocs << endl;
                oss <<"Tracked bytes:" << tracked_bytes << endl; 
            }
            else 
            {
                oss <<"{"; 
                oss <<"Timestamp:" << t_c << ","
                <<"rss:" << process_rss <<","
                <<"virtual:" << process_virtual << ","
                <<"sys_total:" << total_system <<","
                << "sys_free:" << free_system <<","
                <<"TRacked allocs:" << tracked_allocs <<","
                <<"Tracked bytes:" << tracked_bytes << "}";
            }
            return oss.str();
        }
        string MemorySnapshot :: diff(const MemorySnapshot &other) const 
        {
            ostringstream oss; 
            oss << "=== Memory Snapshot Diff ===\n";
            oss << "ΔRSS: " << (long long)process_rss - (long long)other.process_rss << " bytes\n";
            oss << "ΔVirtual: " << (long long)process_virtual - (long long)other.process_virtual << " bytes\n";
            oss << "ΔTracked Allocs: " << (long long)tracked_allocs - (long long)other.tracked_allocs << "\n";
            oss << "ΔTracked Bytes: " << (long long)tracked_bytes - (long long)other.tracked_bytes << " bytes\n";
            return oss.str();
        }

        std::string MemorySnapshot::serialize() const {
    std::ostringstream oss;
    auto t_c = std::chrono::system_clock::to_time_t(timestamp);
    oss << t_c << ";"
        << process_rss << ";"
        << process_virtual << ";"
        << total_system << ";"
        << free_system << ";"
        << tracked_allocs << ";"
        << tracked_bytes;
    return oss.str();
}

optional<MemorySnapshot> MemorySnapshot::deserialize(const std::string& s) 
{
    std::istringstream iss(s);
    MemorySnapshot snap;
    long long t;
    char sep;
    if (!(iss >> t)) return std::nullopt;
    snap.timestamp = std::chrono::system_clock::from_time_t(t);

    if (!(iss >> sep >> snap.process_rss)) return std::nullopt;
    if (!(iss >> sep >> snap.process_virtual)) return std::nullopt;
    if (!(iss >> sep >> snap.total_system)) return std::nullopt;
    if (!(iss >> sep >> snap.free_system)) return std::nullopt;
    if (!(iss >> sep >> snap.tracked_allocs)) return std::nullopt;
    if (!(iss >> sep >> snap.tracked_bytes)) return std::nullopt;

    return snap;
}

    }
}


