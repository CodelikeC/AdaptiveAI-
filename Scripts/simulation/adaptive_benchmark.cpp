#include "adaptive_benchmark.h"
#include "../utils/logger.h"     // Giả định đã có logger
#include <algorithm>
#include <limits>

namespace adaptive_ai {
namespace simulation {

BenchmarkResult AdaptiveBenchmark::run(const std::string& name,
                                       const std::function<void()>& func,
                                       size_t iterations) {
    double totalTime = 0.0;
    double minTime = std::numeric_limits<double>::max();
    double maxTime = 0.0;

    for (size_t i = 0; i < iterations; ++i) 
    {
        auto start = std::chrono::high_resolution_clock::now();

        try 
        {
            func(); // Chạy hàm test
        } 
        catch (const std::exception& ex) 
        {
            Logger::error("[Benchmark] Exception in test '" + name + "': " + ex.what());
        }

        auto end = std::chrono::high_resolution_clock::now();
        double elapsedMs = std::chrono::duration<double, std::milli>(end - start).count();

        totalTime += elapsedMs;
        minTime = std::min(minTime, elapsedMs);
        maxTime = std::max(maxTime, elapsedMs);
    }

    BenchmarkResult result;
    result.testName = name;
    result.avgLatencyMs = totalTime / iterations;
    result.maxLatencyMs = maxTime;
    result.minLatencyMs = minTime;
    result.iterations = iterations;
    result.notes = "Completed without fatal errors.";

    results_.push_back(result);

    Logger::info("[Benchmark] " + name +
                        " avg=" + std::to_string(result.avgLatencyMs) + "ms" +
                        " min=" + std::to_string(result.minLatencyMs) + "ms" +
                        " max=" + std::to_string(result.maxLatencyMs) + "ms" +
                        " (" + std::to_string(iterations) + " iterations)");

    return result;
}

const std::vector<BenchmarkResult>& AdaptiveBenchmark::getResults() const {
    return results_;
}

void AdaptiveBenchmark::reportAll() const 
{
    Logger::info("=== Benchmark Report ===");
    for (const auto& r : results_) 
    {
        Logger::info("Test: " + r.testName +
                            " | avg=" + std::to_string(r.avgLatencyMs) + "ms" +
                            " | min=" + std::to_string(r.minLatencyMs) + "ms" +
                            " | max=" + std::to_string(r.maxLatencyMs) + "ms" +
                            " | iterations=" + std::to_string(r.iterations));
    }
}

} // namespace simulation
} // namespace adaptive_ai

