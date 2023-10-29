cmake_minimum_required (VERSION 3.10)
project ("status-service"
    VERSION 0.0.1
    DESCRIPTION "Simple status-service"
    LANGUAGES CXX)
    
add_subdirectory(third_party)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

add_executable(${PROJECT_NAME} "src/main.cpp")
target_link_libraries(${PROJECT_NAME} PRIVATE hiredis)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_BINARY_DIR}/${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/docker/${PROJECT_NAME}
)