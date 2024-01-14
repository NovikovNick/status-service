project(llhttp VERSION 6.1.0)
set(CMAKE_C_STANDARD 99)

# Source code
set(LLHTTP_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/llhttp.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/http.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/api.c
)

set(LLHTTP_HEADERS
  ${CMAKE_CURRENT_SOURCE_DIR}/include/llhttp.h
)

add_library(${PROJECT_NAME} STATIC)
add_library(llhttp::llhttp ALIAS ${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE ${LLHTTP_SOURCES} ${LLHTTP_HEADERS})
target_include_directories(${PROJECT_NAME} 
INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)