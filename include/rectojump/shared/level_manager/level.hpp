//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_SHARED_LEVEL_MANAGER_LEVEL_HPP
#define RJ_SHARED_LEVEL_MANAGER_LEVEL_HPP


#include "level_background.hpp"
#include "level_info.hpp"
#include <rectojump/global/common.hpp>


namespace rj
{
	struct level_header
	{
		int music_size;
		int background_size;
		int data_size;
		int info_size;
		int reserved;
	};

	struct level
	{
		music_data music;
		level_background background;
		entity_proto_vec entities;
		level_info info;

		bool is_valid() const noexcept
		{return !music.empty() && !entities.empty();}
	};
}


#endif // RJ_SHARED_LEVEL_MANAGER_LEVEL_HPP
