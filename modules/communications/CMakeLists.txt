add_library(communications
    communications/repc_client.cpp
    communications/secure_channel.cpp
    communications/secure_channel.h 
    communications/repc_client.h 
)
target_include_directories(communication PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(communication utils)
