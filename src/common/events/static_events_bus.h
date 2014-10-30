#pragma once

/**/

#include <vector>
#include <algorithm>

#include <boost/mpl/vector.hpp>
#include <boost/mpl/find.hpp>
#include <boost/mpl/size.hpp>
#include <boost/static_assert.hpp>


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
            void handleEvent(IEvent* e)
            {

            }
            virtual ~IReceiver() {}
        };

        template<typename mpl_vector> //typedef boost::fusion::vector<T1,T2>
        class static_event_bus
        {   
            //typedef typename boost::fusion::result_of::as_vector<mpl_vector>::type fusion_vec;
        public:
            enum { count_types = boost::mpl::size<mpl_vector>::value} ;
            static_event_bus()
            {

            }

            template<typename T>
            void registerReciver(IReceiver* receiver)
            {
                BOOST_STATIC_ASSERT(find_in_mpl_vector<T,mpl_vector>::value);
                typedef typename boost::mpl::find<mpl_vector, T>::type MplIter;
                receivers_[MplIter::pos::value].push_back(receiver);
            }

            template<typename T>
            void unregisterReciver(IReceiver* receiver)
            {
                BOOST_STATIC_ASSERT(find_in_mpl_vector<T,mpl_vector>::value);
                typedef typename boost::mpl::find<mpl_vector, T>::type MplIter;
                findAndRemove(MplIter::pos::value, receiver);
            }

            void unregisterReciver(IReceiver* receiver)
            {
                for(int i = 0; i < count_types; ++i){
                    findAndRemove(i, receiver);
                }
            }

            template<typename T>
            void broadcast(T* event)
            {
                BOOST_STATIC_ASSERT(find_in_mpl_vector<T,mpl_vector>::value);
                typedef typename boost::mpl::find<mpl_vector, T>::type MplIter;
                for(int i = 0; i < receivers_[MplIter::pos::value].size(); ++i){
                    IReceiver *rec = receivers_[MplIter::pos::value][i];
                    rec->handleEvent(event);
                }
            }

        private:
            void findAndRemove(unsigned index, IReceiver* receiver)
            {
                std::vector<IReceiver*>::iterator it = std::find_if(receivers_[index].begin(), receivers_[index].end(), receiver);
                if(it != receivers_[index].end()){
                    receivers_[index].erase(it);
                }
            }
            //fusion_vec vec_callbacks_;
            std::vector<IReceiver*> receivers_[count_types];
        };
    }
}

