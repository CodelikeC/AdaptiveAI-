add_library(communication
    communication/repc_client.cpp
    communication/secure_channel.cpp
)
target_include_directories(communication PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(communication utils)
