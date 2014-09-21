#pragma once

/**/

#include <cassert>

#include "common/common_config.h"

#if defined(COMPILER_GCC) || defined(__clang__)
#define WARN_UNUSED_RESULT __attribute__((warn_unused_result))
#else
#define WARN_UNUSED_RESULT
#endif

#define VERIFY(x) assert(x)
#ifdef NDEBUG
#define DCHECK(x)
#define DCHECK_MSG(x, text)
#else
#define DCHECK(x) assert(x)
#define DCHECK_MSG(x, text) assert(x && text)
#endif

#define NOTREACHED() DCHECK(false)
#define NOOP() ((void)0)
#define STRINGIZE_HELPER(x)				#x
#define STRINGIZE(x)					STRINGIZE_HELPER(x)

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);   \
  void operator=(const TypeName&)

#define SIZEOFMASS(type) sizeof(type)/sizeof(*type)
