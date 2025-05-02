
add_executable(test_adaption
    test/test_adaption.cpp
    tests/test_features_extraction.cpp
    tests/test_intrusion.cpp
    
)
target_link_libraries(test_adaption core utils communication)
