CMAKE_MINIMUM_REQUIRED(VERSION 3.0.0)
PROJECT(libmemcached)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_LIST_DIR}/src)

SET(HEADERS_MEMCACHED
    src/mem_config.h
)

SET(SOURCES_MEMCACHED
    src/libmemcached/connect.cc
    src/libmemcached/sasl.cc
    src/libmemcached/version.cc
    src/libmemcached/server.cc
    src/libmemcached/get.cc
    src/libmemcached/strerror.cc
    src/libmemcached/server_list.cc
    src/libmemcached/hosts.cc
    src/libmemcached/delete.cc
    src/libmemcached/memcached.cc
    src/libmemcached/error.cc
    src/libmemcached/instance.cc
    src/libmemcached/io.cc
    src/libmemcached/array.c
    src/libmemcached/storage.cc
    src/libmemcached/response.cc
    src/libmemcached/result.cc
    src/libmemcached/behavior.cc
    src/libmemcached/initialize_query.cc
    src/libmemcached/allocators.cc
    src/libmemcached/quit.cc
    src/libmemcached/do.cc
    src/libmemcached/fetch.cc
    src/libmemcached/string.cc
    src/libmemcached/hash.cc
    src/libmemcached/key.cc
    src/libmemcached/options.cc
    src/libmemcached/flush_buffers.cc
    src/libmemcached/virtual_bucket.c
    src/libmemcached/purge.cc
    src/libmemcached/byteorder.cc
    src/libmemcached/flag.cc
    src/libmemcached/poll.cc
    src/libmemcached/udp.cc
    src/libmemcached/namespace.cc
    src/libmemcached/callback.cc

    src/libmemcached/csl/context.cc
    src/libmemcached/csl/scanner.cc
    src/libmemcached/csl/parser.cc

    src/libhashkit/algorithm.cc
    src/libhashkit/digest.cc
    src/libhashkit/hashkit.cc
    src/libhashkit/encrypt.cc
    src/libhashkit/aes.cc
    src/libhashkit/rijndael.cc
    src/libhashkit/string.cc
    src/libhashkit/jenkins.cc
    src/libhashkit/function.cc
    src/libhashkit/str_algorithm.cc
    src/libhashkit/has.cc
    src/libhashkit/fnv_64.cc
    src/libhashkit/fnv_32.cc
    src/libhashkit/one_at_a_time.cc
    src/libhashkit/crc32.cc
    src/libhashkit/md5.cc
    src/libhashkit/hsieh.cc
    src/libhashkit/murmur.cc
    src/libhashkit/murmur3.cc
    src/libhashkit/murmur3_api.cc
)

SET(HEADERS ${HEADERS} ${HEADERS_MEMCACHED})
SET(SOURCES ${SOURCES} ${SOURCES_MEMCACHED})
ADD_LIBRARY(libmemcached OBJECT ${HEADERS_MEMCACHED} ${SOURCES_MEMCACHED})

