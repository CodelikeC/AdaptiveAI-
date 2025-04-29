#include <iostream> 
using namespace std ; 
#include "logger.h"

namespace adaptive_ai 
{

// Định nghĩa static mutex
std::mutex Logger::log_mutex;

} // namespace adaptive_ai
