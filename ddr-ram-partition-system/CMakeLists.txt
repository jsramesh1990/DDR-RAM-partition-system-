cmake_minimum_required(VERSION 3.10)
project(ddr_ram_system VERSION 1.0.0 LANGUAGES C)

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Include directories
include_directories(src include)

# Source files
set(SOURCES
    src/main.c
    src/ddr_memory.c
    src/gaming_partition.c
    src/rw_partition.c
    src/userspace_app.c
)

# Create executable
add_executable(ddr_ram_system ${SOURCES})

# Set properties
set_target_properties(ddr_ram_system PROPERTIES
    OUTPUT_NAME "ddr_ram_system"
    C_STANDARD 11
)

# Add compile options
target_compile_options(ddr_ram_system PRIVATE
    -Wall
    -Wextra
    -Werror
    -O2
)

# Installation (optional)
install(TARGETS ddr_ram_system DESTINATION bin)
