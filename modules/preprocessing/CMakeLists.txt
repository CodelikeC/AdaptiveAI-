add_library(preprocessing
    data_cleaner.cpp
    data_cleaner.h 
    feature_extraction.h 
    feature_extraction.cpp
    anomaly_pre_filter.cpp 
    contextualizer.cpp 
    data_labeler.cpp 
    pre_feature_validator.cpp 
    temporal_align.cpp 
)
target_include_directories(preprocessing PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(preprocessing utils)

