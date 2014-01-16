#pragma once

#include <vector>
#include <algorithm>

#include <boost/mpl/vector.hpp>
#include <boost/fusion/container/vector.hpp>
#include <boost/fusion/include/as_vector.hpp>
#include <boost/mpl/find.hpp>
#include <boost/static_assert.hpp>
//http://shaderop.com/2010/09/a-simple-intra-process-event-bus-in-c-part-i/


namespace common
{
    namespace events
    {
        template<typename type_t,typename mpl_cont>
        struct find_in_mpl_vector
        {
            typedef typename boost::mpl::find<mpl_cont,type_t>::type it;
            typedef typename boost::mpl::end<mpl_cont>::type end;
            enum{ value = !boost::is_same<it,end>::value };
        };

        class IEvent
        {

        };

        class IReceiver
        {
        public:
            template<typename T>
            void receive(T t, IEvent* e)
            {

            }
            virtual ~IReceiver() {}
        };

        template<typename fusion_vector_t> //typedef boost::fusion::vector<T1,T2>
        class static_event_bus
        {
            typedef fusion_vector_t fusion_vector;
            typedef typename boost::fusion::result_of::as_vector<fusion_vector>::type mpl_vector;
        public:
            static_event_bus()
            {

            }

            template<typename T>
            void registerReciver(T t, IReceiver* receiver)
            {
                BOOST_STATIC_ASSERT(find_in_mpl_vector<T,mpl_vector>::value);
                receivers_.push_back(receiver);
            }

            template<typename T>
            void unregisterReciver(T t, IReceiver* receiver)
            {
                BOOST_STATIC_ASSERT(find_in_mpl_vector<T,mpl_vector>::value);
                std::vector<IReceiver*>::iterator it = std::find_if(receivers_.begin(),receivers_.end(),receiver);
                if(it != receivers_.end()){
                    receivers_.erase(it);
                }
            }

            void unregisterReciver(IReceiver* receiver)
            {

            }

            template<typename T>
            void broadcast(T t, IEvent* event)
            {
                BOOST_STATIC_ASSERT(find_in_mpl_vector<T,mpl_vector>::value);
            }

        private:
            fusion_vector vec_callbacks_;
            std::vector<IReceiver*> receivers_;
        };
    }
}

