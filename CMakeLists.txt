cmake_minimum_required(VERSION 3.13)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Define the path to the Pico SDK
set(PICO_SDK_PATH "${CMAKE_SOURCE_DIR}/external/pico-sdk")

# Add the Pico SDK to your project
include(${PICO_SDK_PATH}/pico_sdk_init.cmake)


project(model_car_controller)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

add_subdirectory(hwControl)
add_subdirectory(httpServer)
add_subdirectory(external)

add_executable(${PROJECT_NAME}
    main.cpp
)

target_compile_definitions(
    httpServer
    PRIVATE PICOHTTPS_WIFI_PASSWORD=\"$ENV{PICOHTTPS_WIFI_PASSWORD}\"
)

link_directories(${CMAKE_SOURCE_DIR}/build)
include_directories(${CMAKE_SOURCE_DIR}/hwControl/runnables/include)
include_directories(${CMAKE_SOURCE_DIR}/hwControl/scheduler/include)
include_directories(${CMAKE_SOURCE_DIR}/httpServer/include)


# Enable USB output, disable UART output 
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(${PROJECT_NAME} pico_stdlib runnables scheduler pico_multicore httpServer)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(${PROJECT_NAME})
