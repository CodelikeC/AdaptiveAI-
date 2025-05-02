add_library(core
    Evolution.h
    Evolution.cpp
    adaption.cpp
    adaption.h 
    meta_learning.cpp 
    meta_learning.h 
    policy.cpp
    policy.h 
)
target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(core utils)
