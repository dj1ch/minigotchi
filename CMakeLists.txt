cmake_minimum_required(VERSION 3.13)

# Including this so we can initialize the sdk
include(pico_sdk_import.cmake)

# Set the project name
project(minigotchi)

# initialize sdk
pico_sdk_init()

# Add the source files
add_executable(${PROJECT_NAME}
    raw80211.cpp
    # Add other source files if needed
)

# Link the PICO SDK libraries
target_link_libraries(${PROJECT_NAME}
    pico_stdlib
    # Add other PICO SDK libraries here
    # (Include the necessary libraries for your project)
)

# Add extra outputs
pico_add_extra_outputs(${PROJECT_NAME})
