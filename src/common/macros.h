#pragma once

/**/

#include <cassert>

#include "common/common_config.h"

// Processor architecture detection.  For more info on what's defined, see:
//   http://msdn.microsoft.com/en-us/library/b0084kay.aspx
//   http://www.agner.org/optimize/calling_conventions.pdf
//   or with gcc, run: "echo | gcc -E -dM -"
#if defined(_M_X64) || defined(__x86_64__)
#define ARCH_CPU_X86_FAMILY 1
#define ARCH_CPU_X86_64 1
#define ARCH_CPU_64_BITS 1
#define ARCH_CPU_LITTLE_ENDIAN 1
#elif defined(_M_IX86) || defined(__i386__)
#define ARCH_CPU_X86_FAMILY 1
#define ARCH_CPU_X86 1
#define ARCH_CPU_32_BITS 1
#define ARCH_CPU_LITTLE_ENDIAN 1
#elif defined(__ARMEL__)
#define ARCH_CPU_ARM_FAMILY 1
#define ARCH_CPU_ARMEL 1
#define ARCH_CPU_32_BITS 1
#define ARCH_CPU_LITTLE_ENDIAN 1
#elif defined(__aarch64__)
#define ARCH_CPU_ARM_FAMILY 1
#define ARCH_CPU_ARM64 1
#define ARCH_CPU_64_BITS 1
#define ARCH_CPU_LITTLE_ENDIAN 1
#elif defined(__pnacl__)
#define ARCH_CPU_32_BITS 1
#define ARCH_CPU_LITTLE_ENDIAN 1
#elif defined(__MIPSEL__)
#define ARCH_CPU_MIPS_FAMILY 1
#define ARCH_CPU_MIPSEL 1
#define ARCH_CPU_32_BITS 1
#define ARCH_CPU_LITTLE_ENDIAN 1
#else
#error Please add support for your architecture in build/build_config.h
#endif

// Type detection for wchar_t.
#if defined(OS_WIN)
#define WCHAR_T_IS_UTF16
#elif defined(OS_POSIX) && (defined(COMPILER_GCC) || defined(COMPILER_CLANG)) && \
    defined(__WCHAR_MAX__) && \
    (__WCHAR_MAX__ == 0x7fffffff || __WCHAR_MAX__ == 0xffffffff)
#define WCHAR_T_IS_UTF32
#define UNICODE
#elif defined(OS_POSIX) && (defined(COMPILER_GCC) || defined(COMPILER_CLANG)) && \
    defined(__WCHAR_MAX__) && \
    (__WCHAR_MAX__ == 0x7fff || __WCHAR_MAX__ == 0xffff)
// On Posix, we'll detect short wchar_t, but projects aren't guaranteed to
// compile in this mode (in particular, Chrome doesn't). This is intended for
// other projects using base who manage their own dependencies and make sure
// short wchar works for them.
#define WCHAR_T_IS_UTF16
#else
#error Please add support for your compiler in build/build_config.h
#endif

#if defined(COMPILER_GCC) || defined(COMPILER_CLANG)
#define WARN_UNUSED_RESULT __attribute__((warn_unused_result))
#define PRINTF_FORMAT(format_param, dots_param) \
    __attribute__((format(printf, format_param, dots_param)))
#else
#define WARN_UNUSED_RESULT
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
