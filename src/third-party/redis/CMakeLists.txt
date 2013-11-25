CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
#PROJECT (redis-cli)

ADD_CUSTOM_TARGET(redis_version
            COMMAND ./mkreleasehdr.sh
            WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/redis/src
            COMMENT "Build redis release.h")

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_LIST_DIR} redis/deps/hiredis/ redis/deps/linenoise/)

SET(HEADERS_REDIS ${CMAKE_CURRENT_LIST_DIR}/redis/src/ae.h ${CMAKE_CURRENT_LIST_DIR}/redis/src/anet.h ${CMAKE_CURRENT_LIST_DIR}/redis/src/crc64.h
${CMAKE_CURRENT_LIST_DIR}/redis/src/fmacros.h ${CMAKE_CURRENT_LIST_DIR}/redis/src/help.h ${CMAKE_CURRENT_LIST_DIR}/redis/deps/hiredis/hiredis.h 
${CMAKE_CURRENT_LIST_DIR}/redis/deps/linenoise/linenoise.h ${CMAKE_CURRENT_LIST_DIR}/redis/deps/hiredis/net.h #redis/src/release.h 
${CMAKE_CURRENT_LIST_DIR}/redis/src/sds.h ${CMAKE_CURRENT_LIST_DIR}/redis/src/version.h ${CMAKE_CURRENT_LIST_DIR}/redis/src/zmalloc.h)

SET(SOURCES_REDIS ${CMAKE_CURRENT_LIST_DIR}/redis/src/ae.c ${CMAKE_CURRENT_LIST_DIR}/redis/src/anet.c ${CMAKE_CURRENT_LIST_DIR}/redis/src/crc64.c
${CMAKE_CURRENT_LIST_DIR}/redis/deps/hiredis/hiredis.c ${CMAKE_CURRENT_LIST_DIR}/redis/deps/linenoise/linenoise.c ${CMAKE_CURRENT_LIST_DIR}/redis/deps/hiredis/net.c 
${CMAKE_CURRENT_LIST_DIR}/redis/src/redis-cli.c ${CMAKE_CURRENT_LIST_DIR}/redis/src/release.c ${CMAKE_CURRENT_LIST_DIR}/redis/src/sds.c ${CMAKE_CURRENT_LIST_DIR}/redis/src/zmalloc.c)

SET(HEADERS ${HEADERS} ${HEADERS_REDIS})
SET(SOURCES ${SOURCES} ${SOURCES_REDIS})
ADD_LIBRARY(redis-cli STATIC ${HEADERS_REDIS} ${SOURCES_REDIS})
ADD_DEPENDENCIES(redis-cli redis_version)
#SET_TARGET_PROPERTIES(redis-cli PROPERTIES LINKER_LANGUAGE C)
