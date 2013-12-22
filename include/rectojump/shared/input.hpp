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

	struct input
	{
		std::map<key, mlk::slot<>> on_key_pressed;

		input() = default;

		void key_pressed(key k)
		{
			if(mlk::cnt::exists_if([=](std::pair<key, mlk::slot<>> p){return p.first == k;}, on_key_pressed))
				on_key_pressed[k]();
		}
	};
}


#endif // RJ_SHARED_INPUT_HPP
