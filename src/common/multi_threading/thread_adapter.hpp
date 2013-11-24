#pragma once

#include "common/multi_threading/common_headers.hpp"
namespace common{
namespace multi_threading 
{
template <class F,void (F::*_Pmemstop)()>
class thread_wrraper_delegate
{
public:
	template <typename A1,typename A2,typename A3>
    explicit thread_wrraper_delegate(A1 a1,A2 a2,A3 a3)
        :m_adapter(a1,a2,a3),m_thread(std::ref(m_adapter))
	{
	}
    template <typename A1,typename A2>
    explicit thread_wrraper_delegate(A1 a1,A2 a2)
        :m_adapter(a1,a2),m_thread(std::ref(m_adapter))
	{
	}

	template <typename A1>
    explicit thread_wrraper_delegate(A1 a1)
        :m_adapter(a1),m_thread(std::ref(m_adapter))
	{
	}
    ~thread_wrraper_delegate()
	{
		(m_adapter.*_Pmemstop)();
		m_thread.join();
	}
private:
	F m_adapter;
	thread_t m_thread;
};

template <class F,void (F::*_Pmemstop)()>
class thread_wrraper_inheritance: public F
{
public:
	template <typename A1,typename A2>
    explicit thread_wrraper_inheritance(A1 a1,A2 a2)
        :F(a1,a2),m_thread(std::ref(*this))
	{
	}
	template <typename A1>
    explicit thread_wrraper_inheritance(A1 a1)
        :F(a1),m_thread(std::ref(*this))
	{
	}
    explicit thread_wrraper_inheritance()
        :F(),m_thread(std::ref(*this))
	{
	}
    ~thread_wrraper_inheritance()
	{
		(this->*_Pmemstop)();
		m_thread.join();
	}
private:
	thread_t m_thread;
};
template <class F>
class thread_wrraper_inheritance_constr: public F
{
public:
	typedef F base_class;
	typedef std::function<void(F&)> type_stop_func;
	template <typename A1,typename A2>
    explicit thread_wrraper_inheritance_constr(A1 a1,A2 a2,type_stop_func stop_func)
        :F(a1,a2),m_thread(std::ref(*this)),_Pmemstop(stop_func)
	{
	}
	template <typename A1>
    explicit thread_wrraper_inheritance_constr(A1 a1,type_stop_func stop_func)
        :F(a1),m_thread(std::ref(*this)),_Pmemstop(stop_func)
	{
	}
    explicit thread_wrraper_inheritance_constr(type_stop_func stop_func)
        :F(),m_thread(std::ref(*this)),_Pmemstop(stop_func)
	{
    }
    ~thread_wrraper_inheritance_constr()
    {
        _Pmemstop(this);
        m_thread.join();
	}
private:
    thread_t m_thread;
	type_stop_func _Pmemstop;
};

}
}
