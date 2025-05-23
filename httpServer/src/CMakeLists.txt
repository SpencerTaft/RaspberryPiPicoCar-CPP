cmake_minimum_required(VERSION 3.13)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Define the path to the Pico SDK
set(PICO_SDK_PATH "${CMAKE_SOURCE_DIR}/external/pico-sdk")

# Add the Pico SDK to your project
include(${PICO_SDK_PATH}/pico_sdk_init.cmake)

project(httpServer)

# Initialize the Raspberry Pi Pico SDK
pico_sdk_init()

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${CMAKE_SOURCE_DIR}/hwControl/runnables/include
    ${CMAKE_SOURCE_DIR}/hwControl/scheduler/include
    ${PICO_SDK_PATH}/lib/lwip/src/include
    ${PICO_SDK_PATH}/lib/lwip/src/include/ipv4
    ${PICO_SDK_PATH}/lib/lwip/src/include/ipv6
    ${PICO_SDK_PATH}/lib/lwip/src/include/arch
    ${PICO_SDK_PATH}/lib/lwip/src/include/lwip
)

add_library(${PROJECT_NAME} STATIC httpServer.cpp)

target_link_libraries(${PROJECT_NAME} pico_stdlib 
    #nlohmann_json::nlohmann_json 
    pico_cyw43_arch_lwip_threadsafe_background
    pico_lwip
    scheduler
)

set_target_properties(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)