#pragma once

/**/

#include <typeinfo>
#include <vector>

#include "common/types.h"

#ifdef BOOST_ENABLED
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
        namespace hash
        {
            uint64_t crc64(uint64_t crc, const byte_type *data, uint64_t lenght);
            uint64_t crc64(uint64_t crc, const buffer_type& data);
        }

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
            inline type findTypeInArray(const unicode_char *(&arr)[size], const char *text)
            {
                for (int i=0; i < size; ++i){
                    if (unicode_strcmp(text,arr[i])==0){
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
                template<typename type_t,typename type_u>
                struct is_same_or_convertible
                {
                    static const bool value = boost::is_same<type_t,type_u>::value || boost::is_convertible<type_t,type_u>::value;
                };
            }

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
        }

        namespace delete_wrappers
        {
            template<typename T>
            inline void destroy(T *&v)
            {
                delete v;
                v = NULL;
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
#ifdef OS_LINUX
        namespace signal
        {
            bool signal(int sig, void (*handler)(int));
            bool sigaddset(sigset_t &signal_mask,int sig);
            bool sigemptyset(sigset_t &signal_mask);
            bool sigprocmask(int how, const sigset_t *set, sigset_t *oset);
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

        template<typename type_u,typename deleter_u,typename type_v,typename deleter_v>
        inline bool operator!=(const base_ptr<type_u, deleter_u>& x,const base_ptr<type_v, deleter_v>& y)
        {
            return x.get() != y.get();
        }

    }
}
