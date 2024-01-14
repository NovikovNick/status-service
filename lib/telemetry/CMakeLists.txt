project (telemetry
    LANGUAGES CXX)

add_compile_options(-Wall -Wextra -Wpedantic)

set(TELEMETRY_SRC 
    private/counter.cc
    private/gauge.cc
    private/histogram.cc
    private/summary.cc)

add_library(${PROJECT_NAME} STATIC ${TELEMETRY_SRC})
add_library (lib::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
target_include_directories(${PROJECT_NAME} 
INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/public
PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/public
)