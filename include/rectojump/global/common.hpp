//
// Copyright (c) 2013 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GLOBAL_COMMON_HPP
#define RJ_GLOBAL_COMMON_HPP


#include <SFML/Window.hpp>


namespace rj
{
	using dur = float;

	using key = sf::Keyboard::Key;
	using btn = sf::Mouse::Button;

	using vec2f = sf::Vector2f;
	using vec2i = sf::Vector2i;
	using vec2u = sf::Vector2u;

	static const std::string data_path{"data/"};
	static const std::string level_path{"levels/"};
}

std::string operator""_data(const char* str, std::size_t)
{return rj::data_path + str;}

std::string operator""_lvl(const char* str, std::size_t)
{return rj::level_path + str;}


#endif // RJ_GLOBAL_COMMON_HPP
