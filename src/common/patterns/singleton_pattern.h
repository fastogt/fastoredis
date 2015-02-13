#pragma once

#include "common/multi_threading/types.h"

namespace common
{
    namespace patterns
    {
        template <class T>
        class Singleton
        {
            static T* _self;
        public:
            void freeInstance();
            static T* instance();

        protected:
            virtual ~Singleton()
            {
                _self = NULL;
            }

            Singleton()
            {
            }
        };

        template <class T>
        T*  Singleton<T>::_self = 0;

        template <class T>
        T*  Singleton<T>::instance()
        {
            if(!_self){
                _self=new T;
            }
            return _self;
        }

        template <class T>
        void  Singleton<T>::freeInstance()
        {
            delete this;
        }

        template <class T>
        class LazySingleton
        {
        public:
            typedef LazySingleton<T> class_type;
            static T &instance();
        protected:
            LazySingleton()
            {
            }

            ~LazySingleton()
            {
            }

        private:
            LazySingleton(class_type const&);
            LazySingleton& operator=(class_type const& rhs);
        };

        template <class T>
        T &LazySingleton<T>::instance()
        {
            static T _self;
            return _self;
        }

        template <class T>
        class TSSingleton
        {
        public:
            static T* instance()
            {
                if(!self_){
                    multi_threading::unique_lock<multi_threading::mutex_t> lock(mutex_);
                    if(!self_){
                        self_ = new T;
                    }
                }
                return self_;
            }

        protected:
            void freeInstance()
            {
                delete this;
            }

            virtual ~TSSingleton()
            {
                self_ = NULL;
            }

            TSSingleton()
            {
            }

        private:
            static T* self_;
            static multi_threading::mutex_t mutex_;
        };

        template <class T>
        T* TSSingleton<T>::self_ = NULL;

        template <class T>
        multi_threading::mutex_t TSSingleton<T>::mutex_;
    }
}
