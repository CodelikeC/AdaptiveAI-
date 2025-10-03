#pragma once 
#include <iostream> 
#include <vector>
#include <string> 

#include <cstddef>
#include <optional> 
#include <mutex> 

using namespace std ;

namespace adaptive_ai{
    namespace memory 
    {
        // snapshot thong tin bo nho tai mot thoi diem.. // 
        struct MemorySnapshot
        {
            size_t process_rss{0}; 
            size_t process_virtual{0}; 
            size_t total_system = 0; 
            size_t free_system = 0; 
            size_t tracked_allocs{0}; 
            size_t tracked_bytes = 0; 
        }; 

        // Memory Introspection : thu thap thong tin bo nho runtime .. .
        class MemoryIntrospection 
        {
            public: 
            MemoryIntrospection(); 

            // Lay snapshot hien tai cua bo nho..
            MemorySnapshot snapshot() const; 

            string report() const; 

            // set so lieu tu allocatorGuard (neu co)
            void set_tracked (size_t allocs, size_t bytes);

            private: 
            mutable mutex m_mutex; 
            size_t m_tracked_allocs = 0; 
            size_t m_tracked_bytes = 0;

            // internal : doc memory usage cua process hien tai .. // 
            MemorySnapshot collect_process_memory() const; 

            // internal : doc system memory info..// 
            void collect_system_memory(MemorySnapshot &snap)const; 
        }; 
    }
}

