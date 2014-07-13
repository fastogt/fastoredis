#include <boost/fusion/include/at.hpp>
#include "common/utils.h"
namespace common{
namespace mpl
{
    template<size_t i,typename... targs_t>
    struct get_variadic_type;

    template<size_t i,typename cur_t,typename... targs_t>
    struct get_variadic_type<i,cur_t,targs_t...>
    {
        typedef typename get_variadic_type<i-1,targs_t...>::curent_type curent_type;
    };
    template<typename cur_t,typename... targs_t>
    struct get_variadic_type<0,cur_t,targs_t...>
    {
        typedef cur_t curent_type;
    };
    struct repiter
    {
        template<typename ...T> repiter(T...) {}//pass{(some_function(args), 1)...};
    };
    namespace get_member
    {
        namespace  detail
        {
            template<typename source_t,typename... targs_t>
            struct get_member_helper
            {
                static_assert(std::is_standard_layout<source_t>::value,"this type have not standard alignment");
                explicit get_member_helper(source_t *src):res_(src)
                {
                }
                enum {size = sizeof...(targs_t)-1, alignment = 4};
                template<size_t i,typename type_t>
                struct loop_helper
                {
                    typedef typename get_variadic_type<size-i,targs_t...>::curent_type current_type;
                    typedef typename get_variadic_type<size-i+1,targs_t...>::curent_type next_type;
                    static size_t get_align(size_t j)
                    {
                        size_t result=0;
                        if(j>0)
                        {
                            size_t current_size = sizeof(current_type);
                            size_t next_size = sizeof(next_type);
                            if(result%alignment!=0&&next_size%alignment==0)
                            {
                                result+=alignment-result%alignment;
                            }
                            else
                            {
                                result+=current_size;
                            }
                            result += loop_helper<i-1,type_t>::get_align(--j);
                        }
                        return result;
                    }
                };
                template<typename type_t>
                struct loop_helper<0,type_t>
                {
                    typedef typename get_variadic_type<size,targs_t...>::curent_type current_type;
                    static size_t get_align(size_t j)
                    {
                        size_t result=0;
                        if(j>0)
                        {
                            size_t current_size = sizeof(current_type);
                            if(result%alignment!=0)
                            {
                                result=alignment-result%alignment;
                            }
                            else
                            {
                                result=current_size;
                            }
                        }
                        return result;
                    }
                };
                void* operator()(size_t j)const
                {
                    size_t result= loop_helper<size,void*>::get_align(j);
                    intptr_t ad = (intptr_t)(res_);
                    return (void*)(ad+result);
                }
            private:
                source_t *res_;
            };
            template<typename source_t,typename... targs_t>
            get_member_helper<source_t,targs_t...> make_get_member_helper(source_t *src)
            {
                return get_member_helper<source_t,targs_t...>(src);
            }
        }
        template<typename helper_t>
        void* get_member(size_t i,const helper_t& h)
        {
            return h(i);
        }
    }
    namespace ariphmetic
    {
        template<typename arg1,typename... args>
        struct get_sizeof
        {
            enum {value = sizeof(arg1)+ get_sizeof<args...>::value};
        };
        template<typename arg>
        struct get_sizeof<arg>
        {
            enum {value = sizeof(arg)};
        };
        template<size_t arg1,size_t... args>
        struct OR
        {
            enum {value =arg1 | OR<args...>::value};
        };
        template<size_t arg>
        struct OR<arg>
        {
            enum {value = arg};
        };
        template<size_t arg1,size_t... args>
        struct AND
        {
            enum {value =arg1 & AND<args...>::value};
        };
        template<size_t arg>
        struct AND<arg>
        {
            enum {value = arg};
        };
    }
    namespace loop
    {
    namespace detail
    {
        template<typename... targs_t>
        struct loop_impl
        {
            enum {size = sizeof...(targs_t)-1};
            template<size_t i,typename type_t>
            struct loop_helper
            {
                typedef typename get_variadic_type<i,targs_t...>::curent_type current_type;
                template<typename func_t>
                static inline void execute(type_t &out,func_t f)
                {
                    boost::fusion::at_c<i>(out) =   utils::converter::make_conversion<current_type>(f(i));
                    loop_helper<i-1,type_t>::execute(out,f);
                }
            };
            template<typename type_t>
            struct loop_helper<0,type_t>
            {
                typedef typename get_variadic_type<0,targs_t...>::curent_type current_type;
                template<typename func_t>
                static inline void execute(type_t &out,func_t f)
                {
                    boost::fusion::at_c<0>(out) = utils::converter::make_conversion<current_type>(f(0));
                }
            };

            template<typename type_t,typename func_t>
            static inline type_t execute(func_t f)
            {
                type_t result;
                loop_helper<size,type_t>::execute(result,f);
                return result;
            }
        };

    }
    template<typename... targs_t>
    struct loop
    {
        template<typename type_t,typename func_t>
        static type_t execute(func_t f)
        {
            return detail::loop_impl<targs_t... >::template execute<type_t>(f);
        }
    };
    }
}
}
