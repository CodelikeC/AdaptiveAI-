add_library(protection
    protection/intrusion_detector.cpp
    protection/intrusion_detector.h 
    protection/self_defense.cpp 
    protection/self_defense.h 
)
target_include_directories(protection PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(protection utils)

