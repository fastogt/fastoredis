#pragma once

/**/

#include "common/common_config.h"

#include <boost/mpl/vector.hpp>
#include <boost/preprocessor.hpp>
#include <boost/mpl/char.hpp>

#define BOOST_PP_ENUM_PARAMS_M_MPL_CHAR(z, n, param) BOOST_PP_COMMA_IF(n) boost::mpl::char_<param ## n>
#define BOOST_PP_ENUM_PARAMS_EX(count, param) BOOST_PP_REPEAT(count, BOOST_PP_ENUM_PARAMS_M_MPL_CHAR, param)

template< BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_MPL_LIMIT_VECTOR_SIZE, char T, 0)>
struct initialize_vector_char;

#define TUPLE_PRINT(n, i, data) data

#define GEN_VECTOR_CHAR(n, i, unused)                                  \
	template< BOOST_PP_ENUM_PARAMS(i, char T) >                            \
struct initialize_vector_char<                                                   \
	BOOST_PP_ENUM_PARAMS(i,T)                                         \
	BOOST_PP_COMMA_IF(i)                                              \
	BOOST_PP_ENUM(                                                    \
	BOOST_PP_SUB(BOOST_MPL_LIMIT_VECTOR_SIZE,i), TUPLE_PRINT, 0) >    \
{                                                                       \
    typedef boost::mpl::vector<BOOST_PP_ENUM_PARAMS_EX(i, T) ,boost::mpl::char_<0> > type; \
    typedef char value_type;   typedef std::basic_string<value_type> string_type;\
};
BOOST_PP_REPEAT_FROM_TO(1, BOOST_MPL_LIMIT_VECTOR_SIZE, GEN_VECTOR_CHAR, ~)

#define BOOST_PP_ENUM_PARAMS_M_MPL_SEQ(z, n, param) BOOST_PP_COMMA_IF(n) BOOST_PP_SEQ_ELEM(n,param)
#define BOOST_PP_ENUM_PARAMS_M_MPL_TYPLE(z, n, param) BOOST_PP_COMMA_IF(n) BOOST_PP_TUPLE_ELEM(BOOST_PP_TUPLE_SIZE(param),n,param)

#define GEN_STRING_TYPLE(X) initialize_vector_char<BOOST_PP_REPEAT(BOOST_PP_TUPLE_SIZE(X), BOOST_PP_ENUM_PARAMS_M_MPL_TYPLE, X)>
#define GEN_STRING_SEQ(X) initialize_vector_char< BOOST_PP_REPEAT(BOOST_PP_SEQ_SIZE(X), BOOST_PP_ENUM_PARAMS_M_MPL_SEQ, X)>
