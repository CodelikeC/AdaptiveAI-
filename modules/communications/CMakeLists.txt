add_library(communications
    demo_communication.cpp
    repc_client.cpp
    secure_channel.cpp
    communication_manager.cpp 
    heartbeat_monitor.cpp 
    channel_factory.cpp 
)
target_include_directories(communication PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(communication utils)
