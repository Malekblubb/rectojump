//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_SHARED_LEVEL_MANAGER_LEVEL_MANAGER_HPP
#define RJ_SHARED_LEVEL_MANAGER_LEVEL_MANAGER_HPP


#include "level.hpp"
#include "packer_pack.hpp"
#include "packer_unpack.hpp"

#include <mlk/filesystem/filesystem.h>

#include <map>
#include <string>


namespace rj
{
	using level_id = std::string;

	class level_manager
	{
		mlk::fs::dir_handle m_dirh;
		mlk::fs::file_handle m_filemgr;
		const std::string& m_abs_path;

		std::map<level_id, level> m_loaded_levels;

	public:
		level_manager(const std::string& abs_level_path) :
			m_dirh{abs_level_path},
			m_abs_path{m_dirh.get_path()}
		{this->init();}

		const level& load_level(const level_id& id)
		{
			this->load_to_mgr(id, this->make_path(id));
			if(m_loaded_levels.find(id) == std::end(m_loaded_levels))
				throw std::runtime_error{"error while open level"};
			return m_loaded_levels[id];
		}

		bool save_level(const level_packer<packer_mode::pack>& lv, const level_id& id)
		{
			if(!m_filemgr.reopen(this->make_path(id), std::ios::out | std::ios::trunc))
				return false;
			m_filemgr.write(lv.packed_data());
			m_filemgr.close();
			return true;
		}

		const level& get_level(const level_id& id) const
		{return m_loaded_levels.at(this->make_id(id));}

		auto get_levels() const noexcept
		-> const decltype(m_loaded_levels)&
		{return m_loaded_levels;}

		std::size_t num_levels() const noexcept
		{return m_loaded_levels.size();}

	private:
		void init()
		{
			mlk::lout("rj::level_manager") << "loading levels recursive from directory '" << m_abs_path << "'...";
			auto content(m_dirh.get_content<true>());
			auto count(0);
			for(auto& a : content)
				if(a.type == mlk::fs::item_type::file)
				{
					this->load_to_mgr(a.name, a.path);
					++count;
				}
			mlk::lout("rj::level_manager") << "loaded " << count << " levels";
		}

		void load_to_mgr(const level_id& id, const std::string& path)
		{
			if(!m_filemgr.reopen(path, std::ios::in))
				return;

			auto data(m_filemgr.read_all());
			level_packer<packer_mode::unpack> unpacker{data};
			m_loaded_levels[id] = unpacker.get_level();
			mlk::lout("rj::level_manager") << "loaded level '" << path << "' " << (m_loaded_levels[id].is_valid() ? "(valid)" : "(invalid)");
		}

		std::string make_path(const level_id& id) const noexcept
		{return m_abs_path + id + ".rjl";}

		level_id make_id(const std::string& str) const noexcept
		{
			auto result(str);
			if(str.find(".rjl") == std::string::npos)
				result += ".rjl";
			return result;
		}
	};
}


#endif // RJ_SHARED_LEVEL_MANAGER_LEVEL_MANAGER_HPP
