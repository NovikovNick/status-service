cmake_minimum_required (VERSION 3.10)
project ("status-service"
    VERSION 0.0.1
    DESCRIPTION "Simple status-service"
    LANGUAGES CXX)
    
add_subdirectory(lib)
add_subdirectory(third_party)

if(CMAKE_BUILD_TYPE STREQUAL "Debug") 	
add_definitions(-DDEBUG) 
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

add_executable(${PROJECT_NAME} "src/main.cc" "src/redis/redis_adapter.cc")
target_link_libraries(${PROJECT_NAME} PRIVATE 
# hiredis 
# lib::telemetry
)

add_executable("server" 
    "src/http-server.cc" 
    "src/server/dispatcher.cc"
    "src/server/http/router.cc"
    "src/server/tcp/connection_pool.cc"
    "src/server/tcp/connection.cc"
    "src/server/service/telemetry.cc")
target_link_libraries("server" PRIVATE lib::telemetry llhttp::llhttp)

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