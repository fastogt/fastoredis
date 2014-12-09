#ifndef _PYTHONQTSTDOUT_H
#define _PYTHONQTSTDOUT_H

#include "PythonQtPythonInclude.h"

#include "structmember.h"
#include <QString>

//! declares the type of the stdout redirection class
extern PyTypeObject PythonQtStdOutRedirectType;

//! declares the callback that is called from the write() function
typedef void PythonQtOutputChangedCB(const QString& str, void* data);

//! declares the stdout redirection class
typedef struct {
  PyObject_HEAD
  PythonQtOutputChangedCB* _cb;
  int softspace;
  void* data;
} PythonQtStdOutRedirect;

#endif
