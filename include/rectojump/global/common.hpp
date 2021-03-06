//
// Copyright (c) 2013-2021 Christoph Malek
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
	{
		main_menu,
		game_menu,
		game,
		editor,
		debug_info,
		error,
		num
	};

	static constexpr const char* state_as_string[std::size_t(state::num)]{
		"main_menu", "game_menu", "game", "editor", "debug_info", "error"};

	enum class game_state : std::size_t
	{
		none,
		pre_running,
		running,
		paused,
		ended,
		num
	};

	// forward
	class game_handler;
	using rndr = render<game_handler>;
	template <typename... T>
	class data_store;
	using data_store_type = data_store<sf::Texture, sf::Font, sf::Font>;

	// general
	using dur = float;

	// input
	using key = sf::Keyboard::Key;
	using btn = sf::Mouse::Button;
	enum class wheel : char
	{
		up,
		down
	};

	// vectors
	using vec2f = sf::Vector2f;
	using vec2i = sf::Vector2i;
	using vec2u = sf::Vector2u;

	// level
	using music_data = mlk::data_packet;
	using entity_prototype = std::vector<float>;
	using entity_proto_vec = std::vector<entity_prototype>;
	enum entity_prototype_value : std::size_t
	{
		figure,
		prop,
		x,
		y
	};

	// level data
	// |-> section headers
	static const mlk::data_packet header_rj_bg{'R', 'J', 'B', 'G'};
	static const mlk::data_packet header_rj_level{'R', 'J', 'L', 'E',
												  'V', 'E', 'L'};
	static const mlk::data_packet header_music{'M', 'U', 'S', 'I', 'C'};

	// |-> strings
	static constexpr const char* level_name_null{"(null)"};

	// ui
	template <typename Textbox_Type>
	void default_textbox(Textbox_Type& tb)
	{
		tb.setOutlineThickness(1.f);
		tb.setOutlineColor(to_rgb("#f15ede"));
		tb.setTextColor(to_rgb("#555555"));
		tb.setTextSize(13);
		tb.setCursorColor(to_rgb("#555555"));
	}

	template <typename Button_Type>
	void default_button(Button_Type& btn)
	{
		btn.setOutlineThickness(1.f);
		btn.setOutlineColor(to_rgb("#f15ede"));
		btn.setFontColor(to_rgb("#555555"));
		btn.setFontSize(13);
	}
}// namespace rj

#endif// RJ_GLOBAL_COMMON_HPP
