CMAKE_MINIMUM_REQUIRED(VERSION 3.0.0)
PROJECT(liblua)

SET(LUA_ROOT_SRC ${CMAKE_CURRENT_LIST_DIR}/lua-5.3.0/src)
INCLUDE_DIRECTORIES(${LUA_ROOT_SRC})

IF(OS_POSIX)
    ADD_DEFINITIONS(-DLUA_USE_POSIX)
ENDIF()

IF(OS_WINDOWS)
    ADD_DEFINITIONS(-DLUA_USE_WINDOWS)
ELSEIF(OS_LINUX)
    ADD_DEFINITIONS(-DLUA_USE_LINUX)
ELSEIF(OS_MACOSX)
    ADD_DEFINITIONS(-LUA_USE_MACOSX)
ELSEIF(OS_ANDROID)
    ADD_DEFINITIONS(-DLUA_ANSI)
ENDIF()

SET(HEADERS_LUA
    ${LUA_ROOT_SRC}/lua.h
    ${LUA_ROOT_SRC}/luaredir.h

    lua_cmsgpack.h
)

SET(SOURCES_LUA
    ${LUA_ROOT_SRC}/lauxlib.c
    ${LUA_ROOT_SRC}/lstrlib.c
    ${LUA_ROOT_SRC}/lbaselib.c
    ${LUA_ROOT_SRC}/linit.c
    ${LUA_ROOT_SRC}/lapi.c
    ${LUA_ROOT_SRC}/luac.c
    ${LUA_ROOT_SRC}/lvm.c
    ${LUA_ROOT_SRC}/liolib.c
    ${LUA_ROOT_SRC}/loadlib.c
    ${LUA_ROOT_SRC}/lmathlib.c
    ${LUA_ROOT_SRC}/lutf8lib.c
    ${LUA_ROOT_SRC}/ltablib.c
    ${LUA_ROOT_SRC}/loslib.c
    ${LUA_ROOT_SRC}/ldblib.c
    ${LUA_ROOT_SRC}/ldebug.c
    ${LUA_ROOT_SRC}/lcorolib.c
    ${LUA_ROOT_SRC}/ldo.c
    ${LUA_ROOT_SRC}/lstate.c
    ${LUA_ROOT_SRC}/lzio.c
    ${LUA_ROOT_SRC}/lparser.c
    ${LUA_ROOT_SRC}/llex.c
    ${LUA_ROOT_SRC}/lctype.c
    ${LUA_ROOT_SRC}/lundump.c
    ${LUA_ROOT_SRC}/ldump.c
    ${LUA_ROOT_SRC}/ltm.c
    ${LUA_ROOT_SRC}/lstring.c
    ${LUA_ROOT_SRC}/lopcodes.c
    ${LUA_ROOT_SRC}/lobject.c
    ${LUA_ROOT_SRC}/lmem.c
    ${LUA_ROOT_SRC}/lcode.c
    ${LUA_ROOT_SRC}/ltable.c
    ${LUA_ROOT_SRC}/lfunc.c
    ${LUA_ROOT_SRC}/lgc.c

    lua_cmsgpack.c
)

ADD_LIBRARY(liblua OBJECT ${HEADERS_LUA} ${SOURCES_LUA})
