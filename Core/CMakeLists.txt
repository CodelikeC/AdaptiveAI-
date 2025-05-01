add_library(core
    data_cleaner.cpp
    feature_extraction.cpp
)
target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(core utils)
