cmake_minimum_required(VERSION 3.31.5)
project(AdaptiveAI-)

# Thiết lập chuẩn C++
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Thêm thư mục source
add_subdirectory(core)
add_subdirectory(modules/sensor)
add_subdirectory(modules/preprocessing)
add_subdirectory(modules/protection)
add_subdirectory(modules/communication)

add_subdirectory(utils)
add_subdirectory(scripts)

# Thêm thư mục test
add_subdirectory(tests)

# Tạo executable chính
add_executable(core/adaption.cpp)
target_link_libraries(core sensor preprocessing protection communication server utils)

# Tạo executable cho các script
add_executable(run_adaptive_core scripts/run_adaptive_core.cpp)
target_link_libraries(run_adaptive_core core utils)

add_executable(demo_sensing scripts/demo_sensing.cpp)
target_link_libraries(demo_sensing sensing utils)

add_executable(test_communication scripts/test_communication.cpp)
target_link_libraries(test_communication communication utils server)
