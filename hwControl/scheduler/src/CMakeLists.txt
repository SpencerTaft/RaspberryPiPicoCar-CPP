project(scheduler)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${CMAKE_SOURCE_DIR}/hwControl/runnables/include
)

add_library(${PROJECT_NAME} STATIC runtimeScheduler.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC runnables)

set_target_properties(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build )