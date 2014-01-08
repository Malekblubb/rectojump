//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_SHARED_LEVEL_MANAGER_LEVEL_HPP
#define RJ_SHARED_LEVEL_MANAGER_LEVEL_HPP


#include "level_info.hpp"
#include <rectojump/global/common.hpp>


namespace rj
{
	struct level_header
	{
		int music_size;
		int data_size;
		int info_size;
		int reserved;
	};

	struct level
	{
		music_data music;
		entity_proto_vec entites;
		level_info info;

		bool is_valid() const noexcept
		{return !music.empty() && !entites.empty();}
	};
}


#endif // RJ_SHARED_LEVEL_MANAGER_LEVEL_HPP
