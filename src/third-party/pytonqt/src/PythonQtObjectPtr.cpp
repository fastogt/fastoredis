#include "PythonQtObjectPtr.h"

PythonQtObjectPtr::PythonQtObjectPtr(PyObject* o)
{
    _object = o;
    if (o) Py_INCREF(_object);
}

PythonQtObjectPtr::~PythonQtObjectPtr()
{ 
    if (_object) Py_DECREF(_object);
}

void PythonQtObjectPtr::setNewRef(PyObject* o)
{
    if (o != _object) {
        if (_object) Py_DECREF(_object);
        _object = o;
    }
}

void PythonQtObjectPtr::setObject(PyObject* o)
{
    if (o != _object) {
        if (_object) Py_DECREF(_object);
        _object = o;
        if (_object) Py_INCREF(_object);
    }
}
