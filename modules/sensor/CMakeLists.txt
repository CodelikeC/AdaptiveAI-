# CMakeLists.txt in modules/sensor

# Tạo thư viện sensor module từ 2 file cpp
add_library(sensor
    network_sensor.cpp
    system_monitor.cpp
    user_behavior_sensor.cpp 
    file_access_sensor.cpp 
    outlier_sensor.cpp 
    registry_sensor.cpp 
)

# Thêm include path đến thư mục utils
target_include_directories(sensor PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../utils
)

# Link thư viện utils nếu bạn đã tạo add_library(utils ...) ở CMakeLists.txt cha
target_link_libraries(sensor 
    utils
)



