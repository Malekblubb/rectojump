//
// Copyright (c) 2013-2017 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_SHARED_UTILS_HPP
#define RJ_SHARED_UTILS_HPP


#include <SFML/Graphics.hpp>

#include <mlk/graphics/color.h>

#include <cmath>


namespace rj
{
	template<typename T>
	T round_to(T value, T to)
	{return (std::round(value / to)) * to;}

	template<typename T>
	sf::Vector2<T> operator-(const sf::Vector2<T>& vec, T minus)
	{return {vec.x - minus, vec.y - minus};}

	template<typename T>
	sf::Vector2<T> operator+(const sf::Vector2<T>& vec, T plus)
	{return {vec.x + plus, vec.y + plus};}

	template<typename T>
	sf::Rect<T> bounds_from_vec(const sf::Vector2<T>& v, const sf::Vector2<T>& size = {1, 1})
	{return {{v.x, v.y}, size};}

	template<typename T>
	void flip_h(T& obj)
	{obj.rotate(180.f);}

	inline sf::Color to_rgb(const std::string& hex_str, std::uint8_t custom_alpha = 255)
	{
		mlk::gcs::color_rgb tmp{hex_str};
		return {tmp.red(), tmp.green(), tmp.blue(), custom_alpha};
	}

	inline sf::Color operator""_rgb(const char* str, std::size_t size)
	{return to_rgb({str, size});}
}


#endif // RJ_SHARED_UTILS_HPP
