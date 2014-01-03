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

	using key_array = std::vector<key>;

	class input
	{
		friend class game_window;

		mlk::bitset<btn, btn::ButtonCount> m_mousebtn_bits;
		mlk::bitset<key, key::KeyCount> m_key_bits;

	public:
		std::map<key, mlk::slot<>> on_key_pressed;
		std::map<std::vector<key>, mlk::slot<>> on_keys_pressed;
		std::map<btn, mlk::slot<const vec2f&>> on_btn_pressed;

		input() = default;

		static input& get() noexcept
		{static input i; return i;}

	private:
		void update(const vec2f& mousepos)
		{
			for(auto& a : on_btn_pressed)
				if(m_mousebtn_bits & a.first)
					a.second(mousepos);

			for(auto& keys : on_keys_pressed)
			{
				auto all_pressed(false);
				for(auto& key : keys.first)
				{
					// check if key 'key' is currently pressed
					if(!(m_key_bits & key))
					{
						all_pressed = false;
						break;
					}
					all_pressed = true;
				}

				// not all keys were pressed
				if(!all_pressed)
					continue;

				// call slot
				keys.second();
			}
		}

		void key_pressed(key k)
		{
			if(mlk::cnt::exists_if(
			[=](const std::pair<key, mlk::slot<>>& p)
			{return p.first == k;}, on_key_pressed))
				on_key_pressed[k]();

			m_key_bits |= k;
		}

		void key_released(key k)
		{m_key_bits.remove(k);}

		void btn_pressed(btn b)
		{m_mousebtn_bits |= b;}

		void btn_released(btn b)
		{m_mousebtn_bits.remove(b);}
	};

	inline auto on_key_pressed(key k)
	-> decltype(input::get().on_key_pressed[k])&
	{return input::get().on_key_pressed[k];}

	template<typename... Keys>
	auto on_keys_pressed(Keys&&... keys)
	-> decltype(input::get().on_keys_pressed[std::vector<key>{}])
	{
		std::vector<key> key_vec;
		mlk::cnt::make_vector(key_vec, std::forward<Keys>(keys)...);
		return input::get().on_keys_pressed[key_vec];
	}

	inline auto on_btn_pressed(btn b)
	-> decltype(input::get().on_btn_pressed[b])&
	{return input::get().on_btn_pressed[b];}
}


#endif // RJ_SHARED_INPUT_HPP
