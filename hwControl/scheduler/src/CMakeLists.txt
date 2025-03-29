set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Define the path to the Pico SDK
set(PICO_SDK_PATH "${CMAKE_SOURCE_DIR}/external/pico-sdk")

   # Add the Pico SDK to your project
include(${PICO_SDK_PATH}/pico_sdk_init.cmake)

project(scheduler)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${CMAKE_SOURCE_DIR}/hwControl/runnables/include
)

add_library(${PROJECT_NAME} STATIC runtimeScheduler.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC runnables pico_stdlib)

set_target_properties(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build )