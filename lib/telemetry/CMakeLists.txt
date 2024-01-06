project (telemetry
    LANGUAGES CXX)

set(LIB_NAME ${PROJECT_NAME})
set(TELEMETRY_SRC 
    counter.cc
    gauge.cc
    histogram.cc
    summary.cc)

add_library(${PROJECT_NAME} STATIC ${TELEMETRY_SRC})
add_library (lib::${LIB_NAME} ALIAS ${LIB_NAME})

add_compile_options(-Wall -Wextra -Wpedantic)

target_compile_features(${LIB_NAME} PUBLIC cxx_std_20)
target_include_directories(${LIB_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/..)
