#pragma once
#include <typeinfo>
#include <tr1/type_traits>
#include <vector>
#include "common/macros.h"
#ifdef __GXX_EXPERIMENTAL_CXX0X__
#include <thread>
#endif
#ifdef BOOST_SUPPORT_ENABLED
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits.hpp>
#include <boost/function.hpp>
#include <boost/lexical_cast.hpp>
#endif
#define HAS_MEMBER(CLASS_NAME,FUNC_MUST_BE) static_assert(std::is_member_function_pointer<decltype(&CLASS_NAME::FUNC_MUST_BE)>::value, "Class does not contain member " #FUNC_MUST_BE "!")
namespace common
{
namespace utils
{
#ifdef __GXX_EXPERIMENTAL_CXX0X__
    namespace raii
    {
        template<typename type_t,type_t (*alloc_f)(),void (*dealloc_f)(type_t)>
        struct scoped_alloc_dealloc
        {
            typedef type_t value_type;
            scoped_alloc_dealloc():value_(alloc_f())
            {
            }
            ~scoped_alloc_dealloc()
            {
                dealloc_f(value_);
            }
            type_t value_;
        };

        template<typename type_t,void (*dealloc_f)(type_t)>
        struct scoped_alloc_dealloc_const
        {
            typedef type_t value_type;
            template<typename aloc_f,typename... args_t>
            scoped_alloc_dealloc_const(aloc_f func,args_t... args):value_(func(args...))
            {

            }
            ~scoped_alloc_dealloc_const()
            {
                dealloc_f(value_);
            }
            type_t value_;
        };
    }
#endif
    namespace traits
    {
        template<typename T>
        struct size_trait_info
        {
            enum {value = sizeof(T) };
        };
        template<size_t N>
        struct size_trait_info<const unicode_char[N] >
        {
            enum {value = N };
        };
    }
    namespace enums
    {
        template<typename type, int size>
        inline type findTypeInArray(const char *(&arr)[size], const char *text)
        {
            for (int i=0; i < size;++i ){
                if (strcmp(text,arr[i])==0){
                    return static_cast<type>(i);
                }
            }
            return static_cast<type>(0);
        }

        template<int size>
        inline std::vector<unicode_string> convertToVector(const char *(&arr)[size])
        {
            std::vector<unicode_string> res;
            for (int i=0; i < size;++i ){
                res.push_back(arr[i]);
            }
            return res;
        }
    }
#ifdef __GXX_EXPERIMENTAL_CXX0X__
    namespace string_utils
    {
        inline unicode_string initilize_string(unicode_string& message)
        {
           return message;
        }
        template<typename TArg, typename... TArgs>
        inline unicode_string initilize_string(unicode_string& message, TArg arg, TArgs... args)
        {
            message+=arg;
            return initilize_string(message, args...);
        }
        template<typename... TArgs>
        inline unicode_string initilize_string(TArgs... args)
        {
            unicode_string message;
            return initilize_string(message,args...);
        }
    }
#endif
    namespace converter
    {
        template < typename cast_to_t, typename cast_from_t >
        inline cast_to_t union_cast( cast_from_t _value )
        {
            union Union{ cast_from_t _from; cast_to_t _to; } unionCast;
            unionCast._from = _value;
            return unionCast._to;
        }
        template<typename convert_to,typename convert_from>
        struct defualt_converter
        {
            convert_to operator()(const convert_from &src)const
            {
                return boost::lexical_cast<convert_to>(src);
            }
        };
        template<typename convert_to,typename convert_from>
        inline convert_to make_conversion(convert_from from)
        {
            return defualt_converter<convert_to,convert_from>()(from);
        }
        namespace detail
        {
#ifdef __GXX_EXPERIMENTAL_CXX0X__
            template<typename type_t>
            struct convert_lamda_type
            {
            private:
                template<typename T>
                static char f(decltype((*(T*)0)()));

                template<typename>
                static long f(...);
            public:
                static const bool value = sizeof( char )==sizeof(f<type_t>( 0 ));
            };
#endif
            template<typename type_t,typename type_u>
            struct is_same_or_convertible
            {
#ifdef __GXX_EXPERIMENTAL_CXX0X__
                static const bool value = std::is_same<type_t,type_u>::value||std::is_convertible<type_t,type_u>::value;
#elif defined(BOOST_SUPPORT_ENABLED)
                static const bool value = boost::is_same<type_t,type_u>::value || boost::is_convertible<type_t,type_u>::value;
#endif
            };
        }
#ifdef __GXX_EXPERIMENTAL_CXX0X__
        template<typename type_t>
        struct variant_defualter
        {
            template<typename const_t>
            explicit variant_defualter(const_t def,typename std::enable_if<detail::is_same_or_convertible<const_t,type_t>::value>::type* v=0):value_(def){}
            template<typename const_t>
            explicit variant_defualter(const_t def,typename std::enable_if<detail::convert_lamda_type<const_t>::value,const_t>::type* v=0):func_(def){}
            struct not_supported_type;
            explicit variant_defualter(not_supported_type* v=0);
            type_t operator()()const
            {
                return func_?func_():value_;
            }
            type_t value_;
            std::function<type_t()> func_;
        };
#elif defined(BOOST_SUPPORT_ENABLED)
        template<typename type_t>
        struct variant_defualter
        {
            template<typename const_t>
            explicit variant_defualter(const_t def,typename boost::enable_if<detail::is_same_or_convertible<const_t,type_t> >::type* v=0):value_(def){}
            struct not_supported_type;
            explicit variant_defualter(not_supported_type* v=0);
            type_t operator()()const
            {
                return func_?func_():value_;
            }
            type_t value_;
            boost::function<type_t()> func_;
        };
#endif
    }
    namespace call_back_definitions
    {
#ifdef __GXX_EXPERIMENTAL_CXX0X__
        template<typename return_t,typename... args>
        struct func_saver
        {
            typedef return_t(*func_type)(args...);
        };
#endif
        template<typename source_t,typename destr_t,typename value_ts,typename value_td,typename return_ts = void>
        struct connection_type_func
        {
            typedef value_ts value_type_source;
            typedef value_td value_type_destr;
            typedef source_t source_type;
            typedef destr_t destr_type;
            typedef return_ts return_type_source;

                    typedef typename std::tr1::add_reference<source_type>::type reference_source_type;
                    typedef typename std::tr1::add_reference<destr_type>::type reference_destr_type;
                    typedef typename std::tr1::add_pointer<source_type>::type pointer_source_type;
                    typedef typename std::tr1::add_pointer<destr_type>::type pointer_destr_type;

            typedef return_type_source (source_t::*set_func_type)(value_type_source);
            typedef value_type_destr (destr_t::*get_func_type)() const;

            connection_type_func(set_func_type set_func=NULL,get_func_type get_func=NULL)
                :ptr_func_set(set_func),ptr_func_get(get_func)
            {

            }
            void operator()(const reference_source_type source,const reference_destr_type destr)
            {
                sync(source,destr,converter::defualt_converter<value_ts,value_td>());
            }
            void operator()(pointer_source_type source,pointer_destr_type destr)
            {
                operator()(*source,*destr);
            }
            template<typename converter_t>
            void operator()(pointer_source_type source,pointer_destr_type destr,converter_t c=0)
            {
                operator()(*source,*destr,c);
            }
            template<typename converter_t>
            void operator()(const reference_source_type source,const reference_destr_type destr,converter_t converter)
            {
                sync(source,destr,converter);
            }
        private:
            template<typename converter_t>
            void sync(const reference_source_type source,const reference_destr_type destr,converter_t converter)
            {
                if(ptr_func_set&&ptr_func_get)
                {
                ((source.*ptr_func_set)(converter(((destr.*ptr_func_get)()))));
                }
            }
            set_func_type ptr_func_set;
            get_func_type ptr_func_get;
        };
        template<typename source_t,typename destr_t,typename value_ts,typename value_td,typename return_ts>
        connection_type_func<source_t,destr_t,value_ts,value_td,return_ts> make_conection_type(return_ts (source_t::*s_ptr)(value_ts),value_td (destr_t::*d_ptr)() const)
        {
            return connection_type_func<source_t,destr_t,value_ts,value_td,return_ts>(s_ptr,d_ptr);
        }
    }
    namespace delete_wrappers
    {
        template<typename T>
        inline void destroy(T *&v)
        {
            delete v;
            v=NULL;
        }
        template <typename T>
        struct default_delete
                :public std::unary_function<T,void>
        {
            inline  void operator ()(T *ptr) const
            {
                  destroy(ptr);
            }
        };

        template <typename T>
        struct default_delete<T*>
                :public std::unary_function<T*,void>
        {
            inline  void operator ()(T *ptr) const
            {
                  destroy(ptr);
            }
        };

        template<typename T,unsigned int N>
        struct default_delete<T[N]>
                :public std::unary_function<const T[N],void>
        {
            inline void operator ()(const T ptr) const
            {
                delete [] ptr;
            }
        };
    }
    namespace compare
    {
    template <typename T>
    struct RuntimeCmp
            :public std::binary_function<const T&,const T&,bool>
    {
        inline bool operator()(const T &t1,const T &t2)const
        {
            return t1<t2;
        }
    };

    template <typename T>
    struct RuntimeCmp<T*>
            :public std::binary_function<const T*,const T*,bool>
    {
        inline bool operator()(const T* t1,const T* t2)const
        {
            return (*t1)<(*t2);
        }
    };

    template<typename T,typename U>
    struct RuntimeCmp<std::pair<T,U> >
        :public std::binary_function<const std::pair<T,U>,const std::pair<T,U>,bool>
    {
        inline bool operator()(const std::pair<T,U> &t1,const std::pair<T,U> &t2)const
        {
                if(t1.first==t2.first)
                {
                    return t1.second<t2.second;
                }
                return t1.first<t2.first;
        }
    };
#ifdef __GXX_RTTI
    template <typename is_type>
    struct run_time_type
    {
        template <typename type>
        inline bool operator()(const type &t)const
        {
            return typeid(t)==typeid(is_type);
        }
        template <typename type>
        inline bool operator()(type *t)const
        {
            return typeid(*t)==typeid(is_type);
        }
    };
#endif
    }
    namespace algoritm
    {
        template<class fwd_it,class type_t>
        inline fwd_it lower_bound_size(fwd_it first, fwd_it last, const type_t& val,size_t size)
        {
            size_t count = size;
            for (; 0 < count; )
            {
                size_t count2 = count / 2;
                fwd_it mid = first;
                std::advance(mid, count2);

                if (*mid < val)
                {
                    first = ++mid;
                    count -= count2 + 1;
                }
                else
                    count = count2;
            }
            return (first);
        }
    }
#ifdef __GXX_EXPERIMENTAL_CXX0X__
    namespace helper
    {
    //typedef std::chrono::high_resolution_clock clock;
        template<typename clock_t>
        void sleep(typename clock_t::time_point start,size_t sleep_time)
        {
            const typename clock_t::time_point stop=start+std::chrono::milliseconds(sleep_time);
            do
            {
                std::this_thread::yield();
            }
            while(clock_t::now()<stop);
            //std::this_thread::sleep_for(std::chrono::duration_cast<std::chrono::milliseconds>(stop - clock_t::now()));
        }
        template<typename clock_t,size_t sleep_time>
        struct sleeper
        {
            sleeper():start(clock_t::now())
            {

            }
            ~sleeper()
            {
                sleep(start,sleep_time);
            }
            const typename clock_t::time_point start;
        };
        template<typename clock_t>
        struct run_time_sleeper
        {
            run_time_sleeper(size_t sleep_time):sleep_time_(sleep_time),start(clock_t::now())
            {

            }
            ~run_time_sleeper()
            {
                sleep<clock_t>(start,sleep_time_);
            }
            size_t sleep_time_;
            const typename clock_t::time_point start;
        };
    }
#endif
#ifdef OS_POSIX
    namespace signal
    {
        bool signal(int sig, void (*handler)(int));
        bool sigaddset(sigset_t &signal_mask,int sig);
        bool sigemptyset(sigset_t &signal_mask);
        bool sigprocmask(int how, const sigset_t *set,sigset_t *oset);
#ifdef __GXX_EXPERIMENTAL_CXX0X__
        struct repiter
        {
            template<typename ...T> repiter(T...) {}//pass{(some_function(args), 1)...};
        };
        template<typename... sign_t>
        inline bool init_sigset(sigset_t *signal_mask,sign_t ...sign)
        {
            bool result = sigemptyset (*signal_mask);
            if(result)
            {
                //#pragma message("TODO: sigaddset return bool must influence to result function")
                repiter{(sigaddset(*signal_mask,sign),1)...};
            }
            return result;
        }
#endif
    }
#endif
}
namespace smart_ptr
{
    template<typename type_t,typename deleter_t = utils::delete_wrappers::default_delete<type_t> >
    class base_ptr
    {
    public:
        typedef type_t* pointer;
        typedef type_t element_type;

        typedef deleter_t deleter_type;
        base_ptr():val_(pointer(),deleter_type())
        {

        }
        explicit base_ptr(pointer p): val_(p, deleter_type())
        {

        }
#ifdef __GXX_EXPERIMENTAL_CXX0X__
        base_ptr(pointer p, typename std::conditional<std::is_reference<deleter_type>::value, deleter_type, const deleter_type&>::type d): val_(p,d)
        {
        }
        base_ptr(pointer p,typename std::remove_reference<deleter_type>::type&& d): val_(std::move(p), std::move(d))
        {
        }
        base_ptr(base_ptr&& u) : val_(u.release(), u.val_.second)
        {
        }
        template<typename type_u, typename deleter_u>
        base_ptr(base_ptr<type_u, deleter_u>&& u) : val_(u.release(), u.val_.second)
        {
        }
#endif
        pointer operator->() const
        {
            return get();
        }
        pointer get() const
        {
            return val_.first;
        }
        void reset(pointer p = pointer())
        {
            if (p != get())
            {
                val_.second(get());
                val_.first = p;
            }
        }
        operator bool() const
        {
            return get() == pointer() ? false : true;
        }
#ifdef __GXX_EXPERIMENTAL_CXX0X__
        base_ptr& operator=(std::nullptr_t)
        {
              reset();
              return *this;
        }
        void swap(base_ptr&& u)
        {
            std::swap(val_, u.val_);
        }
        base_ptr& operator=(base_ptr&& u)
        {
            reset(u.release());
            val_.second = std::move(u.val_.second);
            return *this;
        }
#endif
        pointer release()
        {
            pointer p = get();
            val_.first = NULL;
            return p;
        }
        ~base_ptr()
        {
            reset();
        }
    private:
        base_ptr(const base_ptr&);
        template<typename type_u, typename deleter_u>
        base_ptr(const base_ptr<type_u, deleter_u>&);
        base_ptr& operator=(const base_ptr&);
        template<typename type_u, typename deleter_u>
        base_ptr& operator=(const base_ptr<type_u, deleter_u>&);
        std::pair<pointer,deleter_type> val_;
    };
    template<typename type_u,typename deleter_u,typename type_v,typename deleter_v>
    inline bool operator==(const base_ptr<type_u, deleter_u>& x,const base_ptr<type_v, deleter_v>& y)
    {
        return x.get() == y.get();
    }
#ifdef __GXX_EXPERIMENTAL_CXX0X__
    template<typename type_u,typename deleter_u>
    inline bool operator==(const base_ptr<type_u, deleter_u>& x, std::nullptr_t) noexcept
    {
        return !x;
    }
    template<typename type_u,typename deleter_u>
    inline bool operator==(std::nullptr_t, const base_ptr<type_u, deleter_u>& x) noexcept
    {
        return !x;
    }
    template<typename type_u,typename deleter_u>
    inline bool operator!=(const base_ptr<type_u, deleter_u>& x, std::nullptr_t) noexcept
    {
        return (bool)x;
    }
    template<typename type_u,typename deleter_u>
    inline bool operator!=(std::nullptr_t, const base_ptr<type_u, deleter_u>& x) noexcept
    {
        return (bool)x;
    }
#endif
    template<typename type_u,typename deleter_u,typename type_v,typename deleter_v>
    inline bool operator!=(const base_ptr<type_u, deleter_u>& x,const base_ptr<type_v, deleter_v>& y)
    {
        return x.get() != y.get();
    }

}
}
