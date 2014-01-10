//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_SHARED_UTILS_HPP
#define RJ_SHARED_UTILS_HPP


#include <SFML/Graphics.hpp>

#include <mlk/graphics/color.h>


namespace rj
{
	inline sf::Color to_rgb(const std::string& hex_str)
	{
		mlk::gcs::color_rgb tmp{hex_str};
		return {tmp.red(), tmp.green(), tmp.blue()};
	}

	inline sf::Color operator""_rgb(const char* str, std::size_t size)
	{return to_rgb({str, size});}
}


#endif // RJ_SHARED_UTILS_HPP
