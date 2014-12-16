#pragma once

#include "common/settings/storages/ini_xml_storage.h"
#ifdef OS_WIN
    #include "common/settings/storages/registry_storage.h"
#endif

#include <boost/fusion/container/vector.hpp>
#include <boost/fusion/include/as_vector.hpp>
#include <boost/fusion/container/vector/convert.hpp>
#include <boost/fusion/include/find.hpp>
#include <boost/mpl/count_if.hpp>

//#define TINY_member(z, n, unused) typedef BOOST_PP_CAT(T,n) BOOST_PP_CAT(t,n);=> as mpl::vector member typedef BOOST_PP_REPEAT(FUSION_MAX_VECTOR_SIZE, TINY_member,~)
//#define TINY_params(z, n, unused) class BOOST_PP_CAT(T,n)=void BOOST_PP_ENUM(FUSION_MAX_VECTOR_SIZE, TINY_params, ~) =>BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(FUSION_MAX_VECTOR_SIZE,class T, void)
namespace common{
namespace storages
{
	namespace detail 
	{
		template<typename type_t,typename stor_can>
		struct find_f
		{
			typedef typename stor_can::mpl_vector seq;
			typedef typename boost::mpl::find<seq,type_t>::type it;
			typedef typename boost::mpl::end<seq>::type end;
            enum{ value = !boost::is_same<it,end>::value };
		};

		template<typename type_t,typename type_t2>
		struct is_good_equal_storage_type
		{
            typedef typename boost::is_same<type_t,typename type_t2::storage_type>::type type;
		};		
	}	
	template<typename mpl_vector,typename storage_type>
	struct is_elements_in_same_storage
	{
		typedef mpl_vector sequence; 
        typedef typename boost::mpl::count_if<sequence,detail::is_good_equal_storage_type<storage_type,boost::mpl::_1> >::type type;
        static const bool value = type::value ==(unsigned) boost::mpl::size<sequence>::type::value;
	};

	struct has_nested_id 
	{
        template<typename type_t,class storage_type>
		struct apply 
		{
            enum { value = detail::find_f<type_t,storage_type>::value };
		};
	};
	template<typename type_t,typename strorages_mpl_vector>
	struct contains_in_storages
	{
        typedef typename boost::mpl::find_if<strorages_mpl_vector,typename has_nested_id::apply<type_t,boost::mpl::_1> >::type iterator_type;
		typedef typename boost::mpl::deref<iterator_type>::type container_type; 
        static const bool value = !boost::is_same<iterator_type,typename boost::mpl::end<strorages_mpl_vector>::type>::value;
	};
	template< BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(FUSION_MAX_VECTOR_SIZE,class T, void)>
	struct storage_container_base;

#define GEN_CONTAINER_STORAGE_BASE(n, i, unused)                                  \
	template< BOOST_PP_ENUM_PARAMS(i,class T)>                            \
	struct storage_container_base<BOOST_PP_ENUM_PARAMS(i,T)BOOST_PP_COMMA_IF(i)BOOST_PP_ENUM(BOOST_PP_SUB(FUSION_MAX_VECTOR_SIZE,i), TUPLE_PRINT, void) >    \
	{                                                                       \
		typedef boost::fusion::vector<BOOST_PP_ENUM_PARAMS(i, T)> fusion_vector;\
		typedef typename boost::fusion::result_of::as_vector<fusion_vector>::type mpl_vector;\
	};
	BOOST_PP_REPEAT_FROM_TO(1, FUSION_MAX_VECTOR_SIZE, GEN_CONTAINER_STORAGE_BASE, ~)
#undef GEN_CONTAINER_STORAGE_BASE

////////////////////////////////storage_container///////////////////////////
#define BOOST_PP_ENUM_PARAMS_M_TYPENAME(z, n, param) BOOST_PP_COMMA_IF(n) typename const param ## &n::value_type v##n
#define BOOST_PP_ENUM_PARAMS_M_CONSTR_COMMON(z, n, param) BOOST_PP_COMMA_IF(n) const param ## n &v##n

    template< BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(FUSION_MAX_VECTOR_SIZE, class T, void)>
	struct storage_container;
#define GEN_CONTAINER_STORAGE(n, i, unused)                                  \
	template< BOOST_PP_ENUM_PARAMS(i,class T)>                            \
	struct storage_container<BOOST_PP_ENUM_PARAMS(i,T)BOOST_PP_COMMA_IF(i)BOOST_PP_ENUM(BOOST_PP_SUB(FUSION_MAX_VECTOR_SIZE,i), TUPLE_PRINT, void) >\
		:public storage_container_base<BOOST_PP_ENUM_PARAMS(i,T)BOOST_PP_COMMA_IF(i)BOOST_PP_ENUM(BOOST_PP_SUB(FUSION_MAX_VECTOR_SIZE,i), TUPLE_PRINT, void)>   \
    {                                                                       \
        typedef storage_container_base<BOOST_PP_ENUM_PARAMS(i,T)BOOST_PP_COMMA_IF(i)BOOST_PP_ENUM(BOOST_PP_SUB(FUSION_MAX_VECTOR_SIZE,i), TUPLE_PRINT, void)> base_class;\
        typedef typename T0::storage_type storage_type;\
		template<BOOST_PP_ENUM_PARAMS(i,class const_t)>storage_container(BOOST_PP_REPEAT(i, BOOST_PP_ENUM_PARAMS_M_CONSTR_COMMON, const_t)):vec_(BOOST_PP_ENUM_PARAMS(i,v))\
        {/*BOOST_STATIC_ASSERT_MSG(storages::is_elements_in_same_storage<typename base_class::mpl_vector,storage_type>::value,"diffrent type storages in container");*/}\
		void load(){storage_type::load(vec_);}\
		void save()const{storage_type::save(vec_);}\
        typename base_class::fusion_vector vec_;\
	};
	BOOST_PP_REPEAT_FROM_TO(1, FUSION_MAX_VECTOR_SIZE, GEN_CONTAINER_STORAGE, ~)
#undef GEN_CONTAINER_STORAGE

////////////////////////////////settings_container///////////////////////////
		//BOOST_PP_REPEAT(BOOST_PP_SUB(FUSION_MAX_VECTOR_SIZE,i), FUNC_STORGES,~)template<class type_t> typename boost::disable_if<storages::detail::find_f<type_t,T0>,const not_register_in_storage>::type get_value() const; BOOST_PP_REPEAT(i, FUNC_STORGES,~)
//#define FUNC_STORGES(z, n, unused) template<class type_t> typename boost::enable_if<storages::detail::find_f<type_t,BOOST_PP_CAT(T,n)>,const type_t&>::type 
//	get_value()const{return *boost::fusion::find<type_t>((*boost::fusion::find<BOOST_PP_CAT(T,n)>(vec_conteiner_storages)).vec_);}
	struct load_funct
	{
		template<typename type_t>		
		void operator()(type_t& item)const
		{
			item.load();
		}
	};
	struct save_funct
	{
		template<typename type_t>		
		void operator()(const type_t& item) const
		{
			item.save();
		}
	};

	class not_register_in_storage;
#define BOOST_PP_ENUM_PARAMS_M_CONSTUCTOR(z, n, param) BOOST_PP_COMMA_IF(n) const param ## n &v##n

	template< BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(FUSION_MAX_VECTOR_SIZE,class T, void)>
	struct settings_container;
#define GEN_SETTINGS_CONTAINER(n, i, unused)                                  \
	template< BOOST_PP_ENUM_PARAMS(i,class T)>                            \
	struct settings_container<BOOST_PP_ENUM_PARAMS(i,T)BOOST_PP_COMMA_IF(i)BOOST_PP_ENUM(BOOST_PP_SUB(FUSION_MAX_VECTOR_SIZE,i), TUPLE_PRINT, void) >\
		:public storage_container_base<BOOST_PP_ENUM_PARAMS(i,T)BOOST_PP_COMMA_IF(i)BOOST_PP_ENUM(BOOST_PP_SUB(FUSION_MAX_VECTOR_SIZE,i), TUPLE_PRINT, void)>   \
	{                                                                       \
    settings_container(BOOST_PP_REPEAT(i, BOOST_PP_ENUM_PARAMS_M_CONSTUCTOR, T)):vec_conteiner_storages(BOOST_PP_ENUM_PARAMS(i,v)){reload();}\
		typedef storage_container_base<BOOST_PP_ENUM_PARAMS(i,T)BOOST_PP_COMMA_IF(i)BOOST_PP_ENUM(BOOST_PP_SUB(FUSION_MAX_VECTOR_SIZE,i), TUPLE_PRINT, void)> base_class;\
		template<class type_t> typename boost::disable_if<storages::contains_in_storages<type_t,typename base_class::mpl_vector>,const not_register_in_storage>::type get_value();\
        template<class type_t> typename boost::enable_if<storages::contains_in_storages<type_t,typename base_class::mpl_vector>,type_t&>::type \
        get_value(){return *boost::fusion::find<type_t>((*boost::fusion::find<typename storages::contains_in_storages<type_t,typename base_class::mpl_vector>::container_type>(vec_conteiner_storages)).vec_);}\
        void reload(){boost::fusion::for_each(vec_conteiner_storages, load_funct());}\
    ~settings_container(){boost::fusion::for_each(vec_conteiner_storages, save_funct());}\
        typename base_class::fusion_vector vec_conteiner_storages;\
	};
	BOOST_PP_REPEAT_FROM_TO(1, FUSION_MAX_VECTOR_SIZE, GEN_SETTINGS_CONTAINER, ~)
#undef GEN_SETTINGS_CONTAINER
}
}
