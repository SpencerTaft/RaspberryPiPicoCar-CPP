# Define the path to the Pico SDK
set(PICO_SDK_PATH "${CMAKE_SOURCE_DIR}/external/pico-sdk")

# Add the Pico SDK to your project
include(${PICO_SDK_PATH}/pico_sdk_init.cmake)

project(httpServer)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)

add_library(${PROJECT_NAME} STATIC httpServer.cpp)

target_link_libraries(${PROJECT_NAME} pico_stdlib 
    nlohmann_json::nlohmann_json 
    pico_cyw43_arch_lwip_threadsafe_background
    picow_tcpip_server_poll
    pico_cyw43_arch_lwip_poll
    )

set_target_properties(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build )