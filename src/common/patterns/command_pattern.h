#pragma once

/**/

#include <vector>
#include <algorithm>

namespace common
{
    template<typename Obj, typename T, typename Result>
    inline std::function<Result()> mem_command(Obj obj,Result (T::*_Pm)())
    {
        return std::bind(_Pm,obj);
    }

    struct union_cast
    {
        template<class To>
        struct result_of
        {
            typedef To type;
        };

        template<typename To, typename From>
        static inline typename result_of<To>::type call(From const& from)
        {
            union
            {
                From from;
                To to;
            } u = { from };
            return u.to;
        }
    };

    template<typename Result>
    class ContainerCommand
    {
        typedef std::function<Result()> func;
        typedef std::vector<func> listCommand;
    public:
        template<typename Obj,typename T>
        void addCommand(Obj obj,Result (T::*_Pm)())
        {
            m_listCommand.push_back(mem_command(obj,_Pm));
        }

        template<typename Obj,typename T>
        func removeCommand(Obj obj, Result (T::*_Pm)())
        {
            func result;
            /*typedef decltype(std::bind(_Pm, obj)) bind_t;
            func f = mem_command(obj,_Pm);
            unsigned target = *union_cast::call<unsigned*>(f.target<bind_t>());
            typename listCommand::iterator it;
            for(it = m_listCommand.begin();it!=m_listCommand.end();++it)
            {
                if(*union_cast::call<unsigned*>((*it).target<bind_t>()) == target)
                {
                    result = *it;
                    break;
                }
            }
            m_listCommand.erase(it);*/
            return result;
        }

        void pop()
        {
            if(!m_listCommand.empty())
            {
                func f = *m_listCommand.begin();
                f();
                m_listArhive.push_back(f);
                m_listCommand.erase(m_listCommand.begin());
            }
        }

        void push()
        {
            if(!m_listArhive.empty())
            {
                func f = *m_listArhive.begin();
                f();
                m_listCommand.push_back(f);
                m_listArhive.erase(m_listArhive.begin());
            }
        }

        void clearArhive()
        {
            m_listArhive.clear();
        }

        void execute()
        {
            typename listCommand::iterator it = m_listCommand.begin();
            while(it!=m_listCommand.end())
            {
                (*it++)();
            }
        }

    private:
        listCommand m_listCommand;
        listCommand m_listArhive;
    };
}
