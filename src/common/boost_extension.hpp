#pragma once

#ifdef BOOST_SUPPORT_ENABLED
#include <boost/atomic.hpp>
#include <boost/intrusive_ptr.hpp>
namespace common
{
namespace boost_extension
{
    template<class T,typename refcount_t = boost::atomic_uchar>
    struct intrusive_ptr_base
    {
        typedef refcount_t refcount_type;
        typedef intrusive_ptr_base<T,refcount_t> class_type;
    protected:
        intrusive_ptr_base(): ref_count_(0)
        {
        }
        ~intrusive_ptr_base(){}
        friend void intrusive_ptr_add_ref(class_type const* s)
        {
            ++s->ref_count_;
        }
        friend void intrusive_ptr_release(class_type const* s)
        {
            if (--s->ref_count_ == 0)
            {
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
}
#endif
