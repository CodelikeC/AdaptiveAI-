cmake_minimum_required(VERSION 3.16)
project(AdaptiveAIProject)

# C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Include tất cả các thư mục con
include_directories(
    ${PROJECT_SOURCE_DIR}/core
    ${PROJECT_SOURCE_DIR}/modules/sensing
    ${PROJECT_SOURCE_DIR}/modules/preprocessing
    ${PROJECT_SOURCE_DIR}/modules/protection
    ${PROJECT_SOURCE_DIR}/modules/communication
    ${PROJECT_SOURCE_DIR}/utils
)

# Source files
file(GLOB_RECURSE CORE_SRC
    "core/*.cpp"
)

file(GLOB_RECURSE MODULES_SRC
    "modules/sensing/*.cpp"
    "modules/preprocessing/*.cpp"
    "modules/protection/*.cpp"
    "modules/communication/*.cpp"
)

file(GLOB_RECURSE UTILS_SRC
    "utils/*.cpp"
)

file(GLOB_RECURSE SCRIPTS_SRC
    "scripts/*.cpp"
)

# Targets

# Build thư viện static từ core + modules + utils
add_library(adaptive_ai_core STATIC
    ${CORE_SRC}
    ${MODULES_SRC}
    ${UTILS_SRC}
)

# Các chương trình demo (link thư viện core)
foreach(script ${SCRIPTS_SRC})
    # Lấy tên file không có path và extension
    get_filename_component(script_name ${script} NAME_WE)

    add_executable(${script_name} ${script})
    target_link_libraries(${script_name} adaptive_ai_core)
endforeach()

# Optional: Unit Tests
enable_testing()

file(GLOB TEST_SRC
    "tests/*.cpp"
)

foreach(test_file ${TEST_SRC})
    get_filename_component(test_name ${test_file} NAME_WE)

    add_executable(${test_name} ${test_file})
    target_link_libraries(${test_name} adaptive_ai_core)
    add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()
