add_library(preprocessing
    preprocessing/data_cleaner.cpp
    preprocessing/data_cleaner.h 
    preprocessing/feature_extraction.h 
    preprocessing/feature_extraction.cpp
)
target_include_directories(preprocessing PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(preprocessing utils)

