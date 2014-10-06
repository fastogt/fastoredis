#pragma once

/**/

#include <cassert>

#include "common/common_config.h"

#if defined(COMPILER_GCC) || defined(COMPILER_CLANG)
#define WARN_UNUSED_RESULT __attribute__((warn_unused_result))
#define PRINTF_FORMAT(format_param, dots_param) \
    __attribute__((format(printf, format_param, dots_param)))
#else
#define WARN_UNUSED_RESULT
#define PRINTF_FORMAT(x, y) __attribute__((PRINTF_FORMAT))
#define PRINTF_FORMAT(format_param, dots_param)
#endif

#define VERIFY(x) assert(x)

#ifdef NDEBUG
#define DCHECK(x)
#define DCHECK_MSG(x, text)
#define DCHECK_EQ(x, y)
#define DCHECK_LE(x, y)
#define DCHECK_GT(x, y)
#else
#define DCHECK(x) assert(x)
#define DCHECK_MSG(x, text) assert(x && text)
#define DCHECK_EQ(x, y) assert(x == y)
#define DCHECK_LE(x, y) assert(x < y)
#define DCHECK_GT(x, y) assert(x > y)
#endif

#define NOTREACHED() DCHECK(false)
#define NOOP() ((void)0)
#define STRINGIZE_HELPER(x)				#x
#define STRINGIZE(x)					STRINGIZE_HELPER(x)

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);   \
  void operator=(const TypeName&)

#define SIZEOFMASS(type) sizeof(type)/sizeof(*type)
#define arraysize(type) sizeof(type)/sizeof(*type)

template <bool>
struct CompileAssert {
};

#undef COMPILE_ASSERT
#define COMPILE_ASSERT(expr, msg) \
  typedef CompileAssert<(bool(expr))> msg[bool(expr) ? 1 : -1]
