//
// Copyright (c) 2013 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_SHARED_INPUT_HPP
#define RJ_SHARED_INPUT_HPP


#include <mlk/containers/container_utl.h>
#include <mlk/signals_slots/slot.h>

#include <SFML/Window.hpp>

#include <map>


namespace rj
{
	using key = sf::Keyboard::Key;
	using btn = sf::Mouse::Button;

	struct input
	{
		std::map<key, mlk::slot<>> on_key_pressed;
		std::map<btn, mlk::slot<>> on_btn_pressed;

		input() = default;

		void key_pressed(key k)
		{
			if(mlk::cnt::exists_if([=](std::pair<key, mlk::slot<>> p){return p.first == k;}, on_key_pressed))
				on_key_pressed[k]();
		}

		void btn_pressed(btn b)
		{
			if(mlk::cnt::exists_if([=](std::pair<btn, mlk::slot<>> p){return p.first == b;}, on_btn_pressed))
				on_btn_pressed[b]();
		}
	};
}


#endif // RJ_SHARED_INPUT_HPP
