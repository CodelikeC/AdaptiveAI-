add_library(utils
    utils/logger.cpp
    utils/logger.h 
    utils/config_channel.h 
    utils/config_channel.cpp 
    utils/metrics.cpp
)
target_include_directories(utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
