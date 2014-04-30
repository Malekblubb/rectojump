//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GLOBAL_CONFIG_SETTINGS_HPP
#define RJ_GLOBAL_CONFIG_SETTINGS_HPP


#include "config.hpp"
#include <rectojump/shared/utils.hpp>

#include <mlk/signals_slots/slot.h>
#include <mlk/tools/utils.h>


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

		inline void set_color_light(const std::string& hex)
		{config::get().set_entry("color_light", hex);}

		inline void set_color_dark(const std::string& hex)
		{config::get().set_entry("color_dark", hex);}

		inline void set_color_default_dark(const std::string& hex)
		{config::get().set_entry("color_default_dark", hex);}

		inline void set_color_default_light(const std::string& hex)
		{config::get().set_entry("color_default_light", hex);}

		inline void set_editor_scroll_step(float step)
		{config::get().set_entry("editor_scroll_step", step);}

		inline void set_editor_settings_expanded(bool b)
		{config::get().set_entry("editor_settings_expanded", b);}


		// getters
		template<typename Vec_Type = vec2u, typename Val_Type = decltype(Vec_Type::x)>
		inline Vec_Type get_window_size()
		{return {config::get().get_entry<Val_Type>("window_width"), config::get().get_entry<Val_Type>("window_height")};}

		inline bool get_fullscreen()
		{return config::get().get_entry<bool>("fullscreen");}

		inline int get_sound_volume()
		{return config::get().get_entry<int>("sound_volume");}

		inline sf::Color get_color_light()
		{return to_rgb(config::get().get_entry<std::string>("color_light"));}

		inline sf::Color get_color_dark()
		{return to_rgb(config::get().get_entry<std::string>("color_dark"));}

		inline sf::Color get_color_default_dark()
		{return to_rgb(config::get().get_entry<std::string>("color_default_dark"));}

		inline sf::Color get_color_default_light()
		{return to_rgb(config::get().get_entry<std::string>("color_default_light"));}

		inline float get_editor_scroll_step()
		{return config::get().get_entry<float>("editor_scroll_step");}

		inline bool get_editor_settings_expanded()
		{return mlk::parse_bool(config::get().get_entry<std::string>("editor_settings_expanded"));}


		// fixed settings
		std::string data_path()
		{return "data/";}

		std::string level_path()
		{return "levels/";}

		std::string rj_font()
		{return "Fipps-Regular.otf";}

		std::string text_font()
		{return "Ubuntu-R.ttf";}

		mlk::uint text_size()
		{return 15;}

		inline std::string version()
		{return "0.0.0";}
	}
}


#endif // RJ_GLOBAL_CONFIG_SETTINGS_HPP
