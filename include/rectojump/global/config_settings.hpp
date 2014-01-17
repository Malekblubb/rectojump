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

		inline mlk::slot<>& on_changed()
		{return config::get().on_setting_changed;}


		// setters
		inline void set_window_size(const vec2u& size)
		{
			config::get().set_entry("window_width", size.x);
			config::get().set_entry("window_height", size.y);
			config::get().on_setting_changed();
		}

		inline void set_fullscreen(bool b)
		{
			config::get().set_entry("fullscreen", b);
			config::get().on_setting_changed();
		}

		inline void set_sound_volume(int vol)
		{config::get().set_entry("sound_volume", vol);}


		// getters
		inline vec2u get_window_size()
		{return {config::get().get_entry<mlk::uint>("window_width"), config::get().get_entry<mlk::uint>("window_height")};}

		inline bool get_fullscreen()
		{return config::get().get_entry<bool>("fullscreen");}

		inline int get_sound_volume()
		{return config::get().get_entry<int>("sound_volume");}
	}
}


#endif // RJ_GLOBAL_CONFIG_SETTINGS_HPP
