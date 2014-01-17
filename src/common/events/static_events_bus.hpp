#pragma once

#include <vector>
#include <algorithm>

#include <boost/mpl/vector.hpp>
#include <boost/fusion/container/vector.hpp>
#include <boost/fusion/include/as_vector.hpp>
#include <boost/mpl/find.hpp>
#include <boost/mpl/size.hpp>
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
            enum {count_types = boost::mpl::size<mpl_vector>::size };
            static_event_bus()
            {

            }

            template<typename T>
            void registerReciver(T t, IReceiver* receiver)
            {
                BOOST_STATIC_ASSERT(find_in_mpl_vector<T,mpl_vector>::value);
                typedef boost::mpl::find<mpl_vector, T>::type MplIter;
                receivers_[MplIter::pos::value].push_back(receiver);
            }

            template<typename T>
            void unregisterReciver(T t, IReceiver* receiver)
            {
                BOOST_STATIC_ASSERT(find_in_mpl_vector<T,mpl_vector>::value);
                typedef boost::mpl::find<mpl_vector, T>::type MplIter;
                find_and_remove(MplIter::pos::value, receiver);
            }

            void unregisterReciver(IReceiver* receiver)
            {
                for(int i=0; i < count_types; ++i){
                    findAndRemove(i, receiver);
                }
            }

            template<typename T>
            void broadcast(T t, IEvent* event)
            {
                BOOST_STATIC_ASSERT(find_in_mpl_vector<T,mpl_vector>::value);
                typedef boost::mpl::find<mpl_vector, T>::type MplIter;
                for(int i = 0; i < receivers_[MplIter::pos::value].size(); ++i){
                    IReceiver *rec = receivers_[MplIter::pos::value][i];
                    rec->receive(t, event);
                }
            }

        private:
            void findAndRemove(unsigned index, IReceiver* receiver)
            {
                std::vector<IReceiver*>::iterator it = std::find_if(receivers_[index].begin(),receivers_[index].end(),receiver);
                if(it != receivers_[index].end()){
                    receivers_[index].erase(it);
                }
            }
            //fusion_vector vec_callbacks_;
            std::vector<IReceiver*> receivers_[count_types];
        };
    }
}

