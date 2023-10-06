cmake_minimum_required(VERSION 3.13)

# Set the project name
project(minigotchi)

# Add the source files
add_executable(${PROJECT_NAME}
    raw80211.cpp
)

# Link the PICO SDK libraries
target_link_libraries(${PROJECT_NAME}
    pico_stdlib
    # Add other PICO SDK libraries here
)
