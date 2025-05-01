add_executable(demo_sensing
    scripts/demo_sensing.cpp
)
target_link_libraries(demo_sensing core utils communication)
