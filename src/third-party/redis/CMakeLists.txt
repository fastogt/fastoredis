CMAKE_MINIMUM_REQUIRED(VERSION 3.0.0)
#PROJECT (redis-cli)

ADD_CUSTOM_TARGET(redis_version
            COMMAND ./mkreleasehdr.sh
            WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/redis/src
            COMMENT "Build redis release.h")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_LIST_DIR} redis/deps/hiredis/ redis/deps/linenoise/)

SET(HEADERS_REDIS
    redis/src/ae.h
    redis/src/anet.h
    redis/src/crc64.h
    redis/src/fmacros.h
    redis/src/help.h
    redis/deps/hiredis/hiredis.h
    #redis/deps/linenoise/linenoise.h
    redis/deps/hiredis/net.h
    #redis/src/release.h
    #redis/src/sds.h
    redis/src/version.h
    redis/src/zmalloc.h
	sds.h
)

IF(WIN32)
	INCLUDE_DIRECTORIES(redis/src)
	SET(SOURCES_REDIS_PLATFORM
		net.c
		anet.c
		hiredis.c
	)
ELSE(WIN32)
	SET(SOURCES_REDIS_PLATFORM
		redis/src/anet.c
		redis/deps/hiredis/net.c
		redis/deps/hiredis/hiredis.c
	)
ENDIF(WIN32)

SET(SOURCES_REDIS
    #redis/src/ae.c
    redis/src/crc64.c
    #redis/deps/linenoise/linenoise.c
    #redis/src/redis-cli.c
    redis/src/release.c
    redis/src/sds.c
    redis/src/zmalloc.c
    ${SOURCES_REDIS_PLATFORM}
)

SET(HEADERS ${HEADERS} ${HEADERS_REDIS})
SET(SOURCES ${SOURCES} ${SOURCES_REDIS})
ADD_LIBRARY(redis-cli STATIC ${HEADERS_REDIS} ${SOURCES_REDIS})
IF(WIN32)
    #ADD_DEFINITIONS(-DHAVE_WINSOCK)
    TARGET_LINK_LIBRARIES(redis-cli ws2_32)
ENDIF(WIN32)
ADD_DEPENDENCIES(redis-cli redis_version)
