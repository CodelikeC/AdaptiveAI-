add_library(sensor
    sensor/network_sensor.cpp
    sensor/network_sensor.h
    sensor/system_monitor.cpp  
    sensor/system_monitor.h 
)
target_include_directories(sensor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(logger utils)

