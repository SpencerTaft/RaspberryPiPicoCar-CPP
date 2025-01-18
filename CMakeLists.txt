cmake_minimum_required(VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
#include(pico_sdk_import.cmake)

    # Define the path to the Pico SDK
set(PICO_SDK_PATH "${CMAKE_CURRENT_SOURCE_DIR}/external/pico-sdk")

    # Add the Pico SDK to your project
include(${PICO_SDK_PATH}/pico_sdk_init.cmake)


project(my_project)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project
add_executable(hello_world
    helloWorld.cpp
)

# Enable USB output, disable UART output 
pico_enable_stdio_usb(hello_world 1)
pico_enable_stdio_uart(hello_world 0)

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(hello_world pico_stdlib)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(hello_world)