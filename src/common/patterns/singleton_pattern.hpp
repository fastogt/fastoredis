#pragma once
namespace common
{
namespace patterns
{
    template <class T>
    class singleton
    {
      static T* _self;
    public:
      void free_instance();
      static T* instance();
    protected:
      virtual ~singleton()
      {
          _self = NULL;
      }
      singleton()
      {
      }
    };
    template <class T>
    T*  singleton<T>::_self = NULL;
    template <class T>
    T*  singleton<T>::instance()
    {
      if(!_self)
        _self=new T;
      return _self;
    }
    template <class T>
    void  singleton<T>::free_instance()
    {
        delete this;
    }

    template <class T>
    class lazy_singleton
    {
    public:
      typedef lazy_singleton<T> class_type;
      static T &instance();
    protected:
      lazy_singleton()
      {
      }
      ~lazy_singleton(){}
    private:
      lazy_singleton(class_type const&);
      lazy_singleton& operator=(class_type const& rhs);
    };
    template <class T>
    T &lazy_singleton<T>::instance()
    {
      static T _self;
      return _self;
    }
}
}
