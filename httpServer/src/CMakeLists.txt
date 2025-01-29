cmake_minimum_required(VERSION 3.13)

# Define the path to the Pico SDK
set(PICO_SDK_PATH "${CMAKE_SOURCE_DIR}/external/pico-sdk")

# Add the Pico SDK to your project
include(${PICO_SDK_PATH}/pico_sdk_init.cmake)

project(httpServer)

# Initialize the Raspberry Pi Pico SDK
pico_sdk_init()

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${PICO_SDK_PATH}/lib/lwip/src/include
    ${PICO_SDK_PATH}/lib/lwip/src/include/ipv4
    ${PICO_SDK_PATH}/lib/lwip/src/include/ipv6
    ${PICO_SDK_PATH}/lib/lwip/src/include/arch
)

add_library(${PROJECT_NAME} STATIC httpServer.cpp)

target_link_libraries(${PROJECT_NAME} pico_stdlib 
    nlohmann_json::nlohmann_json 
    pico_cyw43_arch_lwip_threadsafe_background
    picow_tcpip_server_poll
    pico_cyw43_arch_lwip_poll
    lwip
    mbedtls-dev
)

set_target_properties(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)