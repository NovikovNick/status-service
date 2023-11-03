cmake_minimum_required (VERSION 3.10)
project ("status-service"
    VERSION 0.0.1
    DESCRIPTION "Simple status-service"
    LANGUAGES CXX)
    
add_subdirectory(third_party)

if(CMAKE_BUILD_TYPE STREQUAL "Debug") 	
add_definitions(-DDEBUG) 
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

add_executable(${PROJECT_NAME} "src/main.cc" "src/redis_adapter.cc")
target_link_libraries(${PROJECT_NAME} PRIVATE hiredis)


# UTILITY

find_program(DOCKER_COMPOSE_EXECUTABLE docker-compose)
if(NOT DOCKER_COMPOSE_EXECUTABLE)
    message(FATAL_ERROR "Unable to find docker-compose!")
endif()

find_program(DOCKER_EXECUTABLE docker)
if(NOT DOCKER_EXECUTABLE)
    message(FATAL_ERROR "Unable to find docker!")
endif()

add_custom_target(build_docker
    COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_BINARY_DIR}/${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/docker/${PROJECT_NAME}
    COMMAND ${DOCKER_EXECUTABLE} build -t metalheart/${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/docker
)