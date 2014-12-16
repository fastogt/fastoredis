#pragma once

#include <string.h>

#ifdef OS_POSIX
#include <signal.h>
#endif

#include <typeinfo>
#include <vector>

#include "common/types.h"

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
                enum { value = sizeof(T) };
            };
            template<size_t N>
            struct size_trait_info<const char16[N] >
            {
                enum { value = N };
            };
        }

        namespace enums
        {
            template<typename type, int size>
            inline type findTypeInArray(const char16 *(&arr)[size], const char16 *text)
            {
                for (int i=0; i < size; ++i){
                    const size_t len = c16len(text);
                    if (c16memcmp(text, arr[i], len) == 0){
                        return static_cast<type>(i);
                    }
                }
                return static_cast<type>(0);
            }

            template<int size>
            inline std::vector<string16> convertToVector(const char16 *(&arr)[size])
            {
                std::vector<string16> res;
                for (int i = 0; i < size; ++i){
                    res.push_back(arr[i]);
                }
                return res;
            }

            template<typename type, int size>
            inline type findTypeInArray(const char *(&arr)[size], const char *text)
            {
                for (int i=0; i < size; ++i){
                    const size_t len = strlen(text);
                    if (memcmp(text, arr[i], len) == 0){
                        return static_cast<type>(i);
                    }
                }
                return static_cast<type>(0);
            }

            template<int size>
            inline std::vector<std::string> convertToVector(const char *(&arr)[size])
            {
                std::vector<std::string> res;
                for (int i = 0; i < size; ++i){
                    res.push_back(arr[i]);
                }
                return res;
            }
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

        void msleep(unsigned int msec);
        void usleep(unsigned int usec);

#ifdef OS_POSIX
        namespace signal
        {
            bool signal(int sig, void (*handler)(int));
#ifdef OS_LINUX
            bool sigaddset(sigset_t &signal_mask, int sig);
            bool sigemptyset(sigset_t &signal_mask);
            bool sigprocmask(int how, const sigset_t *set, sigset_t *oset);
#endif
        }
#endif
    }
}
