cmake_minimum_required(VERSION 3.13)

# Set the path to the PICO SDK (if not set in the environment)
if (NOT DEFINED ENV{PICO_SDK_PATH})
    # Fetch Pico SDK from git if not set
    set(PICO_SDK_FETCH_FROM_GIT ON CACHE BOOL "Set to ON to fetch copy of SDK from git if not otherwise locatable")
endif()

# Include the Pico SDK import script
include(pico_sdk_import.cmake)

# Initialize the SDK based on PICO_SDK_PATH
pico_sdk_init()

project(minigotchi)

# Add your source files
add_executable(minigotchi
    minigotchi.cpp
    deauthAttack.cpp
    raw80211.cpp
)

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(minigotchi pico_stdlib)

# Create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(minigotchi)
