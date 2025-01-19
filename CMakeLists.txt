cmake_minimum_required(VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
#include(pico_sdk_import.cmake)

    # Define the path to the Pico SDK
set(PICO_SDK_PATH "${CMAKE_CURRENT_SOURCE_DIR}/external/pico-sdk")

    # Add the Pico SDK to your project
include(${PICO_SDK_PATH}/pico_sdk_init.cmake)


project(model_car_controller)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

add_subdirectory(hwControl)
add_subdirectory(httpServer)

# rest of your project
add_executable(${PROJECT_NAME}
    main.cpp
)

# Enable USB output, disable UART output 
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(${PROJECT_NAME} pico_stdlib scheduler runnables httpServer)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(${PROJECT_NAME})
