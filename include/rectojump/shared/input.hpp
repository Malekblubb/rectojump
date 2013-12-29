//
// Copyright (c) 2013 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_SHARED_INPUT_HPP
#define RJ_SHARED_INPUT_HPP


#include <rectojump/global/common.hpp>

#include <mlk/containers/container_utl.h>
#include <mlk/signals_slots/slot.h>

#include <map>


namespace rj
{
	class game_window;

	class input
	{
		friend class game_window;

	public:
		std::map<key, mlk::slot<>> on_key_pressed;
		std::map<btn, mlk::slot<const vec2f&>> on_btn_pressed;

		input() = default;

		static input& get() noexcept
		{static input i; return i;}

	private:
		void key_pressed(key k)
		{
			if(mlk::cnt::exists_if(
			[=](const std::pair<key, mlk::slot<>>& p)
			{return p.first == k;}, on_key_pressed))
				on_key_pressed[k]();
		}

		void btn_pressed(btn b, const vec2f& pos)
		{
			if(mlk::cnt::exists_if(
			[=](const std::pair<btn, mlk::slot<const vec2f&>>& p)
			{return p.first == b;}, on_btn_pressed))
				on_btn_pressed[b](pos);
		}		
	};

	inline auto on_key_pressed(key k)
	-> decltype(input::get().on_key_pressed[k])&
	{return input::get().on_key_pressed[k];}

	inline auto on_btn_pressed(btn b)
	-> decltype(input::get().on_btn_pressed[b])&
	{return input::get().on_btn_pressed[b];}
}


#endif // RJ_SHARED_INPUT_HPP
