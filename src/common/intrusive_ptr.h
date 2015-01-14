#pragma once

#include "common/types.h"

namespace common
{
    template<class T>
    class intrusive_ptr
    {
    private:
        typedef intrusive_ptr this_type;

    public:
        typedef T element_type;

        intrusive_ptr()
            : px(0)
        {
        }

        intrusive_ptr(T * p, bool add_ref = true)
            : px(p)
        {
            if(px != 0 && add_ref) intrusive_ptr_add_ref(px);
        }

        template<class U>
        intrusive_ptr(intrusive_ptr<U> const & rhs)
            : px(rhs.get())
        {
            if(px != 0) intrusive_ptr_add_ref(px);
        }

        intrusive_ptr(intrusive_ptr const & rhs)
            : px(rhs.px)
        {
            if(px != 0) intrusive_ptr_add_ref(px);
        }

        ~intrusive_ptr()
        {
            if(px != 0) intrusive_ptr_release(px);
        }

        template<class U> intrusive_ptr & operator=(intrusive_ptr<U> const & rhs)
        {
            this_type(rhs).swap(*this);
            return *this;
        }

    // Move support

        intrusive_ptr(intrusive_ptr && rhs)
            : px( rhs.px )
        {
            rhs.px = 0;
        }

        intrusive_ptr & operator=(intrusive_ptr && rhs)
        {
            this_type( static_cast< intrusive_ptr && >( rhs ) ).swap(*this);
            return *this;
        }

        intrusive_ptr & operator=(intrusive_ptr const & rhs)
        {
            this_type(rhs).swap(*this);
            return *this;
        }

        intrusive_ptr & operator=(T * rhs)
        {
            this_type(rhs).swap(*this);
            return *this;
        }

        void reset()
        {
            this_type().swap( *this );
        }

        void reset( T * rhs )
        {
            this_type( rhs ).swap( *this );
        }

        void reset( T * rhs, bool add_ref )
        {
            this_type( rhs, add_ref ).swap( *this );
        }

        T * get() const
        {
            return px;
        }

        T * detach()
        {
            T * ret = px;
            px = 0;
            return ret;
        }

        T & operator*() const
        {
            return *px;
        }

        T * operator->() const
        {
            return px;
        }

    // implicit conversion to "bool"

        void swap(intrusive_ptr & rhs)
        {
            T * tmp = px;
            px = rhs.px;
            rhs.px = tmp;
        }

    private:
        T * px;
    };

    template<class T, class U> inline bool operator==(intrusive_ptr<T> const & a, intrusive_ptr<U> const & b)
    {
        return a.get() == b.get();
    }

    template<class T, class U> inline bool operator!=(intrusive_ptr<T> const & a, intrusive_ptr<U> const & b)
    {
        return a.get() != b.get();
    }

    template<class T, class U> inline bool operator==(intrusive_ptr<T> const & a, U * b)
    {
        return a.get() == b;
    }

    template<class T, class U> inline bool operator!=(intrusive_ptr<T> const & a, U * b)
    {
        return a.get() != b;
    }

    template<class T, class U> inline bool operator==(T * a, intrusive_ptr<U> const & b)
    {
        return a == b.get();
    }

    template<class T, class U> inline bool operator!=(T * a, intrusive_ptr<U> const & b)
    {
        return a != b.get();
    }

    template<class T> inline bool operator<(intrusive_ptr<T> const & a, intrusive_ptr<T> const & b)
    {
        return std::less<T *>()(a.get(), b.get());
    }

    template<class T> void swap(intrusive_ptr<T> & lhs, intrusive_ptr<T> & rhs)
    {
        lhs.swap(rhs);
    }

    // mem_fn support

    template<class T> T * get_pointer(intrusive_ptr<T> const & p)
    {
        return p.get();
    }

    template<class T, class U> intrusive_ptr<T> static_pointer_cast(intrusive_ptr<U> const & p)
    {
        return static_cast<T *>(p.get());
    }

    template<class T, class U> intrusive_ptr<T> const_pointer_cast(intrusive_ptr<U> const & p)
    {
        return const_cast<T *>(p.get());
    }

    template<class T, class U> intrusive_ptr<T> dynamic_pointer_cast(intrusive_ptr<U> const & p)
    {
        return dynamic_cast<T *>(p.get());
    }

    // operator<<

    template<class Y> std::ostream & operator<< (std::ostream & os, intrusive_ptr<Y> const & p)
    {
        os << p.get();
        return os;
    }
}

namespace common
{
    template<class T, typename refcount_t = atomic_uchar_t>
    struct intrusive_ptr_base
    {
        typedef refcount_t refcount_type;
        typedef intrusive_ptr_base<T, refcount_t> class_type;

    protected:
        intrusive_ptr_base(): ref_count_(0)
        {
        }

        ~intrusive_ptr_base()
        {
        }

        friend void intrusive_ptr_add_ref(class_type const* s)
        {
            ++s->ref_count_;
        }

        friend void intrusive_ptr_release(class_type const* s)
        {
            if (--s->ref_count_ == 0){
                T const*t = static_cast<T const*>(s);
                delete t;
                t = NULL;
            }
        }

        refcount_type ref_count() const
        {
            return ref_count_;
        }

    private:
        intrusive_ptr_base(class_type const&);
        intrusive_ptr_base& operator=(class_type const& rhs);
        mutable refcount_type ref_count_;
    };
}
