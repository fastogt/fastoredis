#pragma once

/**/

#include <boost/mpl/string.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/replace.hpp>
#include <boost/mpl/remove.hpp>
#include <boost/mpl/for_each.hpp>

namespace common
{
    namespace mpl_vector
    {
        namespace detail
        {
            template <typename begin_iterator, std::size_t index>
            struct string_to_vector_impl
            {
            private:
                static const char past_item = boost::mpl::deref<begin_iterator>::type::value ;
                typedef typename boost::mpl::next<begin_iterator>::type next;
                typedef typename string_to_vector_impl<next, index-1>::type rest;
            public:
                typedef typename boost::mpl::push_front<rest, typename boost::mpl::char_<past_item> >::type type;
            };

            template <typename begin_iterator>
            struct string_to_vector_impl<begin_iterator,1>
            {
                static const char past_item = boost::mpl::deref<begin_iterator>::type::value;
                typedef boost::mpl::vector<typename boost::mpl::char_< past_item > > type;
            };
        }

        template <typename mpl_string>
        struct string_to_vector
        {
            typedef typename detail::string_to_vector_impl<typename boost::mpl::begin<mpl_string>::type, boost::mpl::size<mpl_string>::value>::type type;
        };

        template <typename mpl_vector_char>
        struct vector_to_string
        {
            typedef typename boost::mpl::copy< mpl_vector_char , boost::mpl::inserter<boost::mpl::string<>, boost::mpl::push_back<boost::mpl::_1, boost::mpl::_2> > >::type type;
        };

        template<typename mpl_vector_char,char old_value,char new_value>
        struct replace_char
        {
            typedef typename boost::mpl::replace<mpl_vector_char,boost::mpl::char_<old_value>,boost::mpl::char_<new_value> >::type type;
        };

        template<typename mpl_vector_char,char old_value>
        struct remove_char
        {
            typedef typename boost::mpl::remove<mpl_vector_char,boost::mpl::char_<old_value> >::type type;
        };
    }
}
