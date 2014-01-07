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

#include <string>


namespace rj
{
	class level_manager
	{
		mlk::fs::file_handle m_filemgr;
		std::string m_abs_path;

	public:
		level_manager(const std::string& abs_level_path) :
			m_abs_path{mlk::fs::dir_handle{abs_level_path}.get_path()}
		{ }

		level open_level(const std::string& level_name)
		{
			if(!m_filemgr.reopen(this->make_path(level_name), std::ios::in))
				return level{};

			auto data(m_filemgr.read_all());
			level_packer<packer_mode::unpack> unpacker{data};
			return unpacker.get_level();
		}

		bool save_level(const level_packer<packer_mode::pack>& lv, const std::string& level_name)
		{
			if(!m_filemgr.reopen(this->make_path(level_name), std::ios::out | std::ios::trunc))
				return false;
			m_filemgr.write(lv.packed_data());
			return true;
		}

	private:
		std::string make_path(const std::string& level_name)
		{return m_abs_path + level_name + ".rjl";}
	};
}


#endif // RJ_SHARED_LEVEL_MANAGER_LEVEL_MANAGER_HPP
