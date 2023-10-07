cmake_minimum_required(VERSION 3.13)

# Set the path to the PICO SDK
set(PICO_SDK_PATH ~/minigotchi)

# Include pico_sdk_import.cmake
include(${PICO_SDK_PATH}/pico_sdk_import.cmake)

# Initialize the SDK based on PICO_SDK_PATH
pico_sdk_init()

project(minigotchi)

# Add your source files
add_executable(minigotchi
    raw80211.cpp
)

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(minigotchi pico_stdlib)

# Create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(minigotchi)
