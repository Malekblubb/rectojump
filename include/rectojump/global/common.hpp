//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GLOBAL_COMMON_HPP
#define RJ_GLOBAL_COMMON_HPP


#include <rectojump/core/render.hpp>

#include <mlk/types/types.h>

#include <SFML/Window.hpp>


namespace rj
{
	enum class state : std::size_t
	{main_menu, game_menu, game, editor, debug_info, num};

	// forward
	class game_handler;
	using rndr = render<game_handler>;

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
	{type, prop, x, y};

	// fixed paths
	static const std::string data_path{"data/"};
	static const std::string level_path{"levels/"};

	// other
	inline std::string get_version()
	{return "0.0.0";}
}

inline std::string operator""_data(const char* str, std::size_t)
{return rj::data_path + str;}

inline std::string operator""_lvl(const char* str, std::size_t)
{return rj::level_path + str;}


#endif // RJ_GLOBAL_COMMON_HPP
