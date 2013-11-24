#pragma once
#include <list>
#include <algorithm>
//Obj.*pmf(); template <typename T,T::*pmf()>
namespace common{
template <typename IObserver,typename RetType,typename ArgType, RetType (IObserver::*F)(ArgType) >
class asubject
{
	std::list<IObserver*> list;
public:
    void attach(IObserver *product)
	{
		list.push_back(product);
	}
    void detach(IObserver *product)
	{ 
		list.erase(std::remove(list.begin(), list.end(), product), list.end()); 
	}
    void notify(ArgType val)
	{
		for_each(list.begin(),list.end(),std::bind(F,std::placeholders::_1,price));
	}
};
}
