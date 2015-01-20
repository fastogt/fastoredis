/*
** $Id: lredir_h.h,v 2.106 2015/01/05 13:52:37 roberto Exp $
** Type definitions for Lua objects
** See Copyright Notice in lua.h
*/


#ifndef lredir_h
#define lredir_h

/*
** Hooks to redirect Lua's output to standart streams
*/

#if 1

#include <stdarg.h>
#include <stdio.h>

int (*lua_fprintf)(FILE* stream, const char* msg, ...);
int (*lua_fputs)(const char* msg, FILE* stream);
int (*lua_fwrite)(const void* ptr, size_t size, size_t count, FILE* stream);
int (*lua_printf)(const char* msg, ...);

#define fprintf(...) lua_fprintf(__VA_ARGS__)
#define fputs        lua_fputs
#define fwrite       lua_fwrite
#define printf(...)  lua_printf(__VA_ARGS__)
#endif

#endif
