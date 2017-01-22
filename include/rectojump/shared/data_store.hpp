//
// Copyright (c) 2013-2017 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_SHARED_DATA_STORE_HPP
#define RJ_SHARED_DATA_STORE_HPP


#include <map>
#include <string>
#include <utility>
#include <vector>

#include <mlk/types/types.h>


namespace rj
{
	// TODO: move this helpers ???
	template<std::size_t tup_index, typename Type, typename Func, typename Tup>
	typename std::enable_if<tup_index == std::tuple_size<Tup>::value, void>::type
	get_type_index(Func&& f, Tup& t)
	{ }

	template<std::size_t tup_index, typename Type, typename Func, typename Tup>
	typename std::enable_if<tup_index < std::tuple_size<Tup>::value, void>::type
	get_type_index(Func&& f, Tup& t)
	{
		if(std::is_same<Type, typename std::tuple_element<tup_index, Tup>::type>::value)
			f(tup_index); // type match
		get_type_index<tup_index + 1, Type, Func, Tup>(f, t);
	}

	template<typename Type, typename Func, typename Tup>
	void get_type_index(Func&& f, Tup& t)
	{get_type_index<0, Type>(f, t);}


	struct store_base
	{ };

	template<typename Data_Type>
	struct basic_store : public store_base
	{
		using value_type = Data_Type;
		using map_type = std::map<std::string, value_type>;
		map_type m_values;

		basic_store(const map_type& m) :
			m_values{m}
		{ }
	};

	template<typename... Data_Types>
	class data_store
	{
		using types_tuple = std::tuple<Data_Types...>;
		types_tuple m_helpertuple;
		std::vector<mlk::sptr<store_base>> m_stores;

	public:
		template<typename... Map_Data_Types>
		data_store(Map_Data_Types&&... vars)
		{
			// check number of args
			static_assert(sizeof...(vars) == sizeof...(Data_Types), "rj::data_store<T...>: number of args is not matching");

			this->init(std::forward<Map_Data_Types>(vars)...);
		}

		template<typename Data_Type>
		Data_Type& get(const std::string& id)
		{
			auto index(0);
			get_type_index<Data_Type>([&index](int found_index){index = found_index;}, m_helpertuple);

			auto ptr(std::static_pointer_cast<basic_store<Data_Type>>(m_stores.at(index)));
			return ptr->m_values.at(id);
		}

	private:
		template<typename Head, typename... Tail>
		void init(const std::map<std::string, Head>& h, Tail&&... t)
		{
			auto ptr(std::make_shared<basic_store<Head>>(h));
			m_stores.emplace_back(ptr);
			this->init(std::forward<Tail>(t)...);
		}

		void init()
		{ }
	};
}


#endif // RJ_SHARED_DATA_STORE_HPP
