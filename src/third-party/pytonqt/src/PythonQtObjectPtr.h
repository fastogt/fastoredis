#ifndef _PYTHONQTOBJECTPTR_H
#define _PYTHONQTOBJECTPTR_H

#include "PythonQtPythonInclude.h"

//! a smart pointer that stores a PyObject pointer and that handles reference counting automatically
class PythonQtObjectPtr
{
public:
    PythonQtObjectPtr():_object(NULL) {}

    PythonQtObjectPtr(const PythonQtObjectPtr &p)
        :_object(NULL) {
        setObject(p.object());
    }


    PythonQtObjectPtr(PyObject* o);

    ~PythonQtObjectPtr();

    PythonQtObjectPtr &operator=(const PythonQtObjectPtr &p) {
        setObject(p.object());
        return *this;
    }

    PythonQtObjectPtr &operator=(PyObject* o) {
        setObject(o);
        return *this;
    }


    bool operator==( const PythonQtObjectPtr &p ) const {
        return object() == p.object();
    }

    bool operator!= ( const PythonQtObjectPtr& p ) const {
        return !( *this == p );
    }

    bool operator==( PyObject* p ) const {
        return object() == p;
    }

    bool operator!= ( PyObject* p ) const {
        return object() != p;
    }

    bool isNull() const { return !object(); }

    PyObject* operator->() const { return object(); }

    PyObject& operator*() const { return *( object() ); }

    operator PyObject*() const { return object(); }

//! sets the object and passes the ownership (stealing the reference, in Python slang)
    void setNewRef(PyObject* o);

    PyObject* object() const {
        return _object;
    }

protected:

    void setObject(PyObject* o);

private:
    PyObject* _object;
};

#endif

