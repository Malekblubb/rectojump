//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_SHARED_DATA_MANAGER_HPP
#define RJ_SHARED_DATA_MANAGER_HPP


#include <mlk/filesystem/filesystem.h>
#include <mlk/log/log.h>
#include <mlk/tools/compiletime.h>
#include <mlk/tools/stl_string_utl.h>
#include <mlk/types/types.h>

#include <map>


namespace rj
{
	using data_id = std::string;
	using data_vec = std::vector<mlk::data_packet>;

	class data_manager
	{
		mlk::fs::dir_handle m_dirh;
		mlk::fs::file_handle m_fileh;
		const std::string& m_abs_path;

		std::map<data_id, mlk::data_packet> m_data;

		bool m_valid{false};

	public:
		data_manager(const std::string& abs_datapath, bool auto_load = false) :
			m_dirh{abs_datapath},
			m_abs_path{m_dirh.get_path()},
			m_valid{m_dirh.exists()}
		{
			if(auto_load) this->load_all();
		}

		// interface
		// get:		don't loads the data to manager
		//			just 'gets' it
		auto get_all() const noexcept
		-> const decltype(m_data)&
		{return m_data;}

		mlk::data_packet get_raw(const data_id& id)
		{
			if(!this->exists_id(id))
				return {};
			return m_data[id];
		}

		data_vec get_all_containing_raw(const std::string& contain)
		{
			data_vec result;
			for(auto& a : m_data)
				if(mlk::stl_string::contains(contain, a.first))
					result.emplace_back(this->get_raw(a.first));
			return result;
		}

		template<typename T>
		T get_as(const data_id& id)
		{return this->get_as_impl<T>(id);}

		template<typename... Types, typename... Ids>
		std::tuple<Types...> get_multiple_as(Ids&&... ids)
		{
			static_assert(sizeof...(Types) == sizeof...(Ids), "Amount of types must match the amount of passed ids.");

			std::tuple<Types...> result;
			this->get_multiple_as_impl<mlk::get_upper(-1)>(result, std::forward<Ids>(ids)...);
			return result;
		}

		template<typename Type>
		std::vector<Type> get_all_containing_as(const std::string& contain)
		{
			std::vector<Type> result;
			for(auto& a : m_data)
				if(mlk::stl_string::contains(contain, a.first))
					result.emplace_back(this->get_as_impl<Type>(a.first));
			return result;
		}

		template<typename Type>
		std::map<data_id, Type> get_all_containing_as_map_as(const std::string& contain)
		{
			std::map<data_id, Type> result;
			for(auto& a : m_data)
				if(mlk::stl_string::contains(contain, a.first))
					result.emplace(a.first, this->get_as_impl<Type>(a.first));
			return result;

		}

		// load:	gets AND loads the data to manager
		mlk::data_packet load_raw(const data_id& id)
		{
			this->load_raw_impl(id, this->make_path(id));
			return m_data[id];
		}

		template<typename T>
		T load_as(const data_id& id)
		{return this->load_as_impl<T>(id);}

		template<typename... Types, typename... Ids>
		std::tuple<Types...> load_multiple_as(Ids&&... ids)
		{
			static_assert(sizeof...(Types) == sizeof...(Ids), "Amount of types must match the amount of passed ids.");

			std::tuple<Types...> result;
			this->load_multiple_as_impl<mlk::get_upper(-1)>(result, std::forward<Ids>(ids)...);
			return result;
		}

		bool exists_id(const data_id& id) const noexcept
		{return m_data.find(id) != std::end(m_data);}

		bool exists_ids(const std::initializer_list<data_id>& il) const noexcept
		{
			for(auto& a : il)
				if(!mlk::cnt::exists_map_first(a, m_data))
					return false;
			return true;
		}

		std::size_t num_data() const noexcept
		{return m_data.size();}

	private:
		// utils
		std::string make_path(const data_id& id)
		{return m_abs_path + id;}

		std::size_t get_datasize() const noexcept
		{
			std::size_t result{0};
			for(auto& a : m_data) result += a.second.size();
			return result;
		}

		// loads all data recursive
		// from absolute directory
		void load_all()
		{
			mlk::lout("rj::data_manager") << "loading files recursive from directory '" << m_abs_path << "'...";
			auto content(m_dirh.get_content<true>());
			auto count(0);
			for(auto& a : content)
				if(a.type == mlk::fs::item_type::file)
				{
					this->load_raw_impl(a.name, a.path);
					++count;
				}
			mlk::lout("rj::data_manager") << "loaded " << count << " files (" << this->get_datasize() << " bytes)";
		}


		// tuple impls
		template<int tup_index, typename Id_Head, typename... Types, typename... Id_Tail>
		void get_multiple_as_impl(std::tuple<Types...>& tup, const Id_Head& head, Id_Tail&&... tail)
		{
			std::get<tup_index>(tup) = this->get_as_impl<typename std::tuple_element<tup_index, std::tuple<Types...>>::type>(head);
			this->get_multiple_as_impl<mlk::get_upper(tup_index)>(tup, std::forward<Id_Tail>(tail)...);
		}

		template<int tup_index, typename... Types>
		void get_multiple_as_impl(std::tuple<Types...> &tup)
		{/* case: no args; do nothing */}

		template<int tup_index, typename Id_Head, typename... Types, typename... Id_Tail>
		void load_multiple_as_impl(std::tuple<Types...>& tup, const Id_Head& head, Id_Tail&&... tail)
		{
			std::get<tup_index>(tup) = this->load_as_impl<typename std::tuple_element<tup_index, std::tuple<Types...>>::type>(head);
			this->load_multiple_as_impl<mlk::get_upper(tup_index)>(tup, std::forward<Id_Tail>(tail)...);
		}

		template<int tup_index, typename... Types>
		void load_multiple_as_impl(std::tuple<Types...>& tup)
		{/* case: no args; do nothing */}


		// lowest level impls
		void load_raw_impl(const data_id& id, const std::string& path)
		{
			if(this->exists_id(id))
			{
				mlk::lerr()["rj::data_manager"] << "object with id '" << id << "' already loaded";
				return;
			}
			m_fileh.reopen(path, std::ios::in);
			m_data[id] = m_fileh.read_all();
			mlk::lout("rj::data_manager") << "loaded data '" << path << "'";
		}

		template<typename T>
		T load_as_impl(const data_id& id)
		{
			this->load_raw_impl(id, this->make_path(id));
			return this->get_as_impl<T>(id);
		}

		template<typename T>
		T get_as_impl(const data_id& id)
		{
			if(!this->exists_id(id))
			{
				mlk::lerr()["rj::data_manager"] << "object with id '" << id << "' not found";
				return T{};
			}
			T result;
			result.loadFromMemory(m_data[id].data(), m_data[id].size());
			return result;
		}
	};
}


#endif // RJ_SHARED_DATA_MANAGER_HPP
