#include "common/macros.h"
#include "common/mpl.hpp"
#include <list>
namespace common{
namespace patterns
{
    template<class AbstractProduct,typename IdentifierType,class ProductCreator = AbstractProduct* (*)()>
    class Factory
    {
    public:
        Factory(const Factory&) = delete;
        Factory& operator=(const Factory&) = delete;
        void registerItem(const IdentifierType &val,ProductCreator creator)
        {
           m_factory.insert( FactoryMap::value_type(val,creator));
        }
        AbstractProduct *CreateObject(const IdentifierType &val)
        {
            AbstractProduct *result = nullptr;
            typename FactoryMap::const_iterator it =m_factory.find(val);
            if(it!=m_factory.end())
            {
                AbstractProduct *cur = it->second();
                m_listCreatedItems.push_back(cur);
                result =  cur;
            }
           return result;// throw std::runtime_error("Wrong name");
        }
        ~Factory()
        {
            m_factory.clear();
            for_each(m_listCreatedItems.begin(),m_listCreatedItems.end(),utils::delete_wrappers::default_delete<AbstractProduct>());
        }
    private:
        typedef std::map<IdentifierType,ProductCreator> FactoryMap;
        FactoryMap m_factory;
        std::list<AbstractProduct*> m_listCreatedItems;
    };

    template<size_t i,typename... types>
    struct create_item_impl
    {
        typedef typename mpl::get_variadic_type<i,types...>::curent_type curent_type;
        template<typename... targs_t>
        static void* create(const unicode_char *str,targs_t ...args)
        {
            if(unicode_strcmp(curent_type::name,str)==0)
            {
                return new typename curent_type::type(args...);
            }
            else
            {
                return create_item_impl<i-1,types...>::create(str,args...);
            }
        }
    };
    template<typename... types>
    struct create_item_impl<0,types...>
    {
        typedef typename mpl::get_variadic_type<0,types...>::curent_type curent_type;
        template<typename... targs_t>
        static void* create(const unicode_char *str,targs_t ...args)
        {
            if(unicode_strcmp(curent_type::name,str)==0)
            {
                return new typename curent_type::type(args...);
            }
            else
            {
                return nullptr;
            }
        }
    };

    template<typename... types>
    struct create_item
    {
        enum {size = sizeof...(types)-1};
        template<typename... targs_t>
        static void* create(const char *str,targs_t ...args)
        {
            return create_item_impl<size,types...>::create(str,args...);
        }
    };

    template<typename... types>
    struct static_factory
    {
        template<typename... targs_t>
        void* find_and_create(const unicode_char * val,targs_t ...args)
        {
            return create_item<types...>::create(val,args...);
        }
    };
}
}
