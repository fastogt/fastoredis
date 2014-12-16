#pragma once

// Undefine macros that Python.h defines to avoid redefinition warning.
#undef _POSIX_C_SOURCE
#undef _POSIX_THREADS
#undef _XOPEN_SOURCE

// Undefine Qt keywords that conflict with Python headers
#ifdef slots
#undef slots
#define PYTHONQT_RESTORE_KEYWORDS
#endif

//in pyconfig.h, the 64 bit flag is only set for the Microsoft Compiler
#ifdef _WIN64
#define MS_WIN64
#endif

// If PYTHONQT_USE_RELEASE_PYTHON_FALLBACK is enabled, try to link
// release Python DLL if it is available by undefining _DEBUG while
// including Python.h
#if defined(PYTHONQT_USE_RELEASE_PYTHON_FALLBACK) && defined(_DEBUG)
#undef _DEBUG
#if defined(_MSC_VER) && _MSC_VER >= 1400
#define _CRT_NOFORCE_MANIFEST 1
#define _STL_NOFORCE_MANIFEST 1
#endif
#include <Python.h>
#define _DEBUG
#else
#include <Python.h>
#endif

// get Qt keywords back
#ifdef PYTHONQT_RESTORE_KEYWORDS
#define slots Q_SLOTS
#undef PYTHONQT_RESTORE_KEYWORDS
#endif

#if PY_MAJOR_VERSION >= 3
#define PY3K
// Helper defines to facilitate porting
#define PyString_FromString PyUnicode_FromString
#define PyString_AS_STRING  PyUnicode_AsUTF8
#define PyString_AsString   PyUnicode_AsUTF8
#define PyString_FromFormat PyUnicode_FromFormat
#define PyString_Check      PyUnicode_Check

#define PyInt_Type     PyLong_Type
#define PyInt_FromLong PyLong_FromLong
#define PyInt_AS_LONG  PyLong_AS_LONG
#define PyInt_Check    PyLong_Check
#define PyInt_AsLong   PyLong_AsLong

#else
// Defines to use Python 3 names in Python 2 code
#define PyBytes_Type      PyString_Type
#define PyBytes_Check     PyString_Check
#define PyBytes_AS_STRING PyString_AS_STRING
#define PyBytes_AsString  PyString_AsString
#define PyBytes_GET_SIZE  PyString_GET_SIZE
#define PyBytes_FromStringAndSize PyString_FromStringAndSize
#endif
