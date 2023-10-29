project (hiredis
    LANGUAGES C)

set(HIREDIS_SRC
    alloc.c
    async.c
    hiredis.c
    net.c
    read.c
    sds.c
    sockcompat.c)

add_library (${PROJECT_NAME} STATIC ${HIREDIS_SRC})
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})
