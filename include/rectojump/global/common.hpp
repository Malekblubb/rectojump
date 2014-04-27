//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GLOBAL_COMMON_HPP
#define RJ_GLOBAL_COMMON_HPP


#include <rectojump/core/render.hpp>
#include <rectojump/shared/utils.hpp>

#include <mlk/types/types.h>

#include <SFML/Window.hpp>


namespace rj
{	
	enum class state : std::size_t
	{main_menu, game_menu, game, editor, debug_info, error, num};

	// forward
	class game_handler;
	using rndr = render<game_handler>;
	template<typename... T> class data_store;
	using data_store_type = data_store<sf::Texture, sf::Font, sf::Font>;

	// general
	using dur = float;

	using key = sf::Keyboard::Key;
	using btn = sf::Mouse::Button;
	enum class wheel : char
	{up, down};

	using vec2f = sf::Vector2f;
	using vec2i = sf::Vector2i;
	using vec2u = sf::Vector2u;

	// level
	using music_data = mlk::data_packet;
	using entity_prototype = std::vector<float>;
	using entity_proto_vec = std::vector<entity_prototype>;
	enum entity_prototype_value : std::size_t
	{figure, prop, x, y};

	namespace glob
	{
		// fixed paths
		static const std::string data_path{"data/"};
		static const std::string level_path{"levels/"};
		static const std::string rj_font{"Fipps-Regular.otf"};
		static const std::string text_font{"Ubuntu-R.ttf"};
		static const mlk::uint text_size{15};

		// other
		inline std::string get_version()
		{return "0.0.0";}
	}

	template<typename Textbox_Type>
	void default_textbox(Textbox_Type& tb)
	{
		tb.setOutlineThickness(1.f);
		tb.setOutlineColor(to_rgb("#f15ede"));
		tb.setTextColor(to_rgb("#555555"));
		tb.setTextSize(13);
		tb.setCursorColor(to_rgb("#555555"));
	}

	template<typename Button_Type>
	void default_button(Button_Type& btn)
	{
		btn.setOutlineThickness(1.f);
		btn.setOutlineColor(to_rgb("#f15ede"));
		btn.setFontColor(to_rgb("#555555"));
		btn.setFontSize(13);
	}
}

inline std::string operator""_data(const char* str, std::size_t)
{return rj::glob::data_path + str;}

inline std::string operator""_lvl(const char* str, std::size_t)
{return rj::glob::level_path + str;}


#endif // RJ_GLOBAL_COMMON_HPP
