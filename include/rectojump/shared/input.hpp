//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_SHARED_INPUT_HPP
#define RJ_SHARED_INPUT_HPP


#include <rectojump/global/common.hpp>

#include <mlk/containers/container_utl.h>
#include <mlk/signals_slots/slot.h>
#include <mlk/tools/bitset.h>

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
		mlk::bitset<btn, btn::ButtonCount> m_mousebtn_bits;

		input() = default;

		static input& get() noexcept
		{static input i; return i;}

	private:
		void update(const vec2f& mousepos)
		{
			for(auto& a : on_btn_pressed)
				if(m_mousebtn_bits & a.first)
					a.second(mousepos);
		}

		void key_pressed(key k)
		{
			if(mlk::cnt::exists_if(
			[=](const std::pair<key, mlk::slot<>>& p)
			{return p.first == k;}, on_key_pressed))
				on_key_pressed[k]();
		}

		void btn_pressed(btn b) noexcept
		{m_mousebtn_bits |= b;}

		void btn_released(btn b) noexcept
		{m_mousebtn_bits.remove(b);}
	};

	inline auto on_key_pressed(key k)
	-> decltype(input::get().on_key_pressed[k])&
	{return input::get().on_key_pressed[k];}

	inline auto on_btn_pressed(btn b)
	-> decltype(input::get().on_btn_pressed[b])&
	{return input::get().on_btn_pressed[b];}
}


#endif // RJ_SHARED_INPUT_HPP
