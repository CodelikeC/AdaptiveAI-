
add_executable(tests 
    test_adaption.cpp
    test_features_extraction.cpp
    test_intrusion.cpp
    
)
target_link_libraries(test_adaption core utils communication)
