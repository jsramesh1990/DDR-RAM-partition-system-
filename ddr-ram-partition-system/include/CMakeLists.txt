cmake_minimum_required(VERSION 3.10)
project(ddr_ram_system VERSION 1.0.0 LANGUAGES C)

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Include directories
include_directories(src include)

# Source files for main executable
set(MAIN_SOURCES
    src/main.c
    src/ddr_memory.c
    src/gaming_partition.c
    src/rw_partition.c
    src/userspace_app.c
)

# Source files for tests
set(TEST_SOURCES
    tests/test_main.c
    tests/test_runner.c
    src/ddr_memory.c
    src/gaming_partition.c
    src/rw_partition.c
    src/userspace_app.c
)

# Create main executable
add_executable(ddr_ram_system ${MAIN_SOURCES})

# Create test executable
add_executable(ddr_test_suite ${TEST_SOURCES})

# Set properties for main executable
set_target_properties(ddr_ram_system PROPERTIES
    OUTPUT_NAME "ddr_ram_system"
    C_STANDARD 11
)

# Set properties for test executable
set_target_properties(ddr_test_suite PROPERTIES
    OUTPUT_NAME "ddr_test_suite"
    C_STANDARD 11
)

# Add compile options for both
foreach(target ddr_ram_system ddr_test_suite)
    target_compile_options(${target} PRIVATE
        -Wall
        -Wextra
        -Werror
        -O2
    )
endforeach()

# Enable testing
enable_testing()

# Add test
add_test(NAME ddr_test_suite COMMAND ddr_test_suite)

# Installation (optional)
install(TARGETS ddr_ram_system DESTINATION bin)
