CMAKE_MINIMUM_REQUIRED(VERSION 3.0.0)
PROJECT(redis-cli)

ADD_CUSTOM_TARGET(redis_version
            COMMAND ./mkreleasehdr.sh
            WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/src
            COMMENT "Build redis release.h")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_LIST_DIR}/deps/hiredis ${CMAKE_CURRENT_LIST_DIR}/src)

SET(HEADERS_REDIS    
    src/anet.h
    src/crc64.h
    src/release.h
    src/zmalloc.h
    src/config.h

    deps/hiredis/fmacros.h
)

IF(WIN32)
    SET(REDIS_PLATFORM_SRCS deps/hiredis/net_win.c deps/hiredis/hiredis_win.c)
ELSE()
    SET(REDIS_PLATFORM_SRCS deps/hiredis/net.c deps/hiredis/hiredis.c)
ENDIF(WIN32)

SET(SOURCES_REDIS
    src/anet.c
    src/crc64.c
    src/release.c
    src/zmalloc.c

    deps/hiredis/sds.c
    ${REDIS_PLATFORM_SRCS}
)

SET(HEADERS ${HEADERS} ${HEADERS_REDIS})
SET(SOURCES ${SOURCES} ${SOURCES_REDIS})
ADD_LIBRARY(redis-cli STATIC ${HEADERS_REDIS} ${SOURCES_REDIS})
IF(WIN32)
    TARGET_LINK_LIBRARIES(redis-cli ws2_32)
ENDIF(WIN32)
ADD_DEPENDENCIES(redis-cli redis_version)
