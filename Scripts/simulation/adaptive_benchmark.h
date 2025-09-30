#pragma once 
#include <iostream> 
#include <vector> 
#include <string> 
#include <functional>
#include <chrono> 

using namespace std;

namespace adaptive_ai 
{
    namespace simulation
    {
        struct BenchmarkResult{
            string testName; 
            double avgLatencyMs; 
            double maxLatencyMs; 
            double minLatencyMs; 
            size_t iterations; 

            string notes; 
        }; 
        class AdaptiveBenchmark 
        {
            public:
            
            // Chay mot benchmark voi ten va ham test..//
            BenchmarkResult run(const string &name,
            const function<void()> &func, size_t iterations = 100);
            
            // Tra ve danh sach ket qua da chay.. // 
            const vector<BenchmarkResult> &getResults()const; 

            // In toan bo ket qua benchmark.. ra console log...// 
            void reportAll() const; 

            private: 
            vector<BenchmarkResult> results_; 
        }; 
    }
}
