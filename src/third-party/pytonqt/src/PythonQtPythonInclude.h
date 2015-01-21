/*
 *
 *  Copyright (C) 2011 MeVis Medical Solutions AG All Rights Reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  Further, this software is distributed without any warranty that it is
 *  free of the rightful claim of any third person regarding infringement
 *  or the like.  Any license provided herein, whether implied or
 *  otherwise, applies only to this software file.  Patent licenses, if
 *  any, provided herein do not apply to combinations of this program with
 *  other software, or any other product whatsoever.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *  Contact information: MeVis Medical Solutions AG, Universitaetsallee 29,
 *  28359 Bremen, Germany or:
 *
 *  http://www.mevis.de
 *
 */

#ifndef __PythonQtPythonInclude_h
#define __PythonQtPythonInclude_h

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

#endif
