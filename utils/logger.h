#pragma once

#include <iostream>
#include <mutex>
#include <sstream>

namespace adaptive_ai {

class Logger
{
public:
    template<typename... Args>
    static void info(Args&&... args)
    {
        log("[INFO]", std::forward<Args>(args)...);
    }

    template<typename... Args>
    static void warn(Args&&... args)
    {
        log("[WARN]", std::forward<Args>(args)...);
    }

    template<typename... Args>
    static void error(Args&&... args)
    {
        log("[ERROR]", std::forward<Args>(args)...);
    }

    template<typename...Args>
    static void debug(Args&&... args)
    {
        debug("Debug", forward<Args>(args)...);
    }

private:
    static std::mutex log_mutex;

    template<typename... Args>
    static void log(const std::string& level, Args&&... args)
    {
        std::lock_guard<std::mutex> lock(log_mutex);
        std::ostringstream oss;
        (oss << ... << args);
        std::cout << level << " " << oss.str() << std::endl;
    }
};

// Định nghĩa static member
inline std::mutex Logger::log_mutex;

} // namespace adaptive_ai
