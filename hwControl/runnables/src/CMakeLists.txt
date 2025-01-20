project(runnables)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)

add_library(${PROJECT_NAME} STATIC lightRunnable.cpp)

set_target_properties(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build )
