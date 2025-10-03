#pragma once 
#include <iostream> 
#include <vector>
#include <string> 

#include <cstddef>
#include <mutex> 
#include <optional>
#include <unordered_map>

using namespace std; 

namespace adaptive_ai
{
    namespace memory
    {
        struct AllocationInfo
        {
            size_t size; 
            const char *file; 
            int line; 
        }; 

        class AllocatorGuard
        {
            public: 
            AllocatorGuard();
            ~AllocatorGuard();

            // allocate/free (tracked)
            void* allocate(size_t size, const char* file = nullptr, int line = 0);
            void deallocate(void* ptr);

            // rollback all active allocations
            void rollback();

            // stats
            size_t active_allocations() const;
            size_t active_bytes() const;
            size_t total_allocations() const;

    // dump current leaks (returns formatted string)
            string dump_leaks() const;

private:
    mutable mutex m_mutex;
    unordered_map<void*, AllocationInfo> m_map;
    size_t m_total_allocs{0};
    size_t m_active_bytes{0};
        };
        // convenient macros for tracking file/line
#define GUARD_ALLOC(guard, size) (guard.allocate(size, __FILE__, __LINE__))
#define GUARD_FREE(guard, ptr)   (guard.deallocate(ptr))
    }
}
