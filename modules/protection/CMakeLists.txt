add_library(protection
    intrusion_detector.cpp
    self_defense.cpp 
    self_defense.h 
    threat_analyzer.cpp 
    trust_guard.cpp 
    killswitch_monitor.cpp 
)
target_include_directories(protection PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(protection utils)


