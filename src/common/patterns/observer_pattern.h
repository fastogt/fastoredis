#pragma once

/**/

#include <list>
#include <algorithm>
//Obj.*pmf(); template <typename T,T::*pmf()>

namespace common
{
    template <typename IObserver, typename RetType, typename ArgType, RetType (IObserver::*F)(ArgType)>
    class asubject
    {
        std::list<IObserver*> list_;

    public:
        void attach(IObserver *product)
        {
            list_.push_back(product);
        }

        void detach(IObserver *product)
        {
            list_.erase(std::remove(list_.begin(), list_.end(), product), list_.end());
        }

        void notify(ArgType val)
        {
            for_each(list_.begin(),list_.end(),std::bind(F, std::placeholders::_1, val));
        }
    };
}
