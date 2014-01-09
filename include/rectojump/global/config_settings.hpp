//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GLOBAL_CONFIG_SETTINGS_HPP
#define RJ_GLOBAL_CONFIG_SETTINGS_HPP


#include "config.hpp"


namespace rj
{
	namespace settings
	{
		inline void init()
		{config::get();}

		inline int sound_volume()
		{return config::get().get_entry<int>("sound_volume");}

		inline void set_sound_volume(int vol)
		{config::get().set_entry("sound_volume", vol);}
	}
}


#endif // RJ_GLOBAL_CONFIG_SETTINGS_HPP
