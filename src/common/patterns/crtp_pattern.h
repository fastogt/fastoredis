#pragma once

/**/

namespace common
{
    template<typename T>
    class object_counter
    {
    public:
        static unsigned int count()
        {
            return object_counter<T>::_count;
        }

    protected:
        object_counter()
        {
            ++object_counter<T>::_count;
        }

        object_counter(const object_counter<T> &)
        {
            ++object_counter<T>::_count;
        }

        ~object_counter()
        {
            --object_counter<T>::_count;
        }

    private:
        static unsigned int _count; //not thread safe
    };

    template<typename T>
    unsigned int object_counter<T>::_count =0;
}
