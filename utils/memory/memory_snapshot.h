#pragma once 
#include <iostream>
#include <vector> 
#include <cstddef>

#include <string> 
#include <chrono>
#include <optional>

using namespace std; 

namespace adaptive_ai 
{
    namespace memory
    {
        struct MemorySnapshot{
            chrono::system_clock::time_point timestamp;
            size_t process_rss = 0;
            size_t process_virtual = 0;
            size_t total_system = 0;
            size_t free_system = 0;
            size_t tracked_allocs = 0;
            size_t tracked_bytes = 0;

            // Convert to JSON like string.. //
            string to_string(bool pretty = true);

            // compare with another snapshot..//
            string diff (const MemorySnapshot &other) const;

            // Serialize to a compact string(for presistemce)
            string serialize() const; 

            // Deserialize from a string...// 
            static optional<MemorySnapshot> deserialize(const string &s); 
        };


    }
}
