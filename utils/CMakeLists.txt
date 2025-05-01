add_library(utils
    logger.cpp
)
target_include_directories(utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
