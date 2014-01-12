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

	using key_vec = std::vector<key>;

	class input
	{
		friend class game_window;

		mlk::bitset<btn, btn::ButtonCount> m_mousebtn_bits;
		mlk::bitset<key, key::KeyCount> m_key_bits;

		mlk::event_delegates<key> m_on_key_pressed;
		mlk::event_delegates<key_vec> m_on_keys_pressed;
		mlk::event_delegates<btn, const vec2f&> m_on_btn_pressed;
		mlk::event_delegates<wheel, const vec2f&> m_on_mousewheel;

	public:
		input() = default;

		static input& get() noexcept
		{static input i; return i;}

		bool is_key_valid(key k) const noexcept
		{return k != key::Unknown;}

	private:
		void update(const vec2f& mousepos)
		{
			for(auto& a : m_on_btn_pressed)
				if(m_mousebtn_bits & a.first)
					a.second(mousepos);
		}

		void key_pressed(key k)
		{
			if(!this->is_key_valid(k))
				return;

			if(mlk::cnt::exists_if(
			[=](const std::pair<key, mlk::slot<>>& p)
			{return p.first == k;}, m_on_key_pressed))
				m_on_key_pressed[k]();

			m_key_bits |= k;
			for(auto& keys : m_on_keys_pressed)
			{
				auto all_pressed(false);
				for(auto& key : keys.first)
				{
					if(!this->is_key_valid(key))
						break;

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

		void key_released(key k)
		{
			if(!this->is_key_valid(k))
				return;
			m_key_bits.remove(k);
		}

		void btn_pressed(btn b)
		{m_mousebtn_bits |= b;}

		void btn_released(btn b)
		{m_mousebtn_bits.remove(b);}

		void mousewheel_moved(int delta, const vec2f& pos)
		{delta < 0 ? m_on_mousewheel[wheel::down](pos) : m_on_mousewheel[wheel::up](pos);}


		friend auto on_key_pressed(key k)
		-> decltype(m_on_key_pressed[k])&;

		template<typename... Keys>
		friend auto on_keys_pressed(Keys&&...)
		-> decltype(m_on_keys_pressed[key_vec{}])&;

		friend bool is_key_pressed(key);

		friend void simulate_keypress(key);

		friend auto on_btn_pressed(btn b)
		-> decltype(m_on_btn_pressed[b])&;

		friend bool is_btn_pressed(btn);

		friend void simulate_btnpress(btn);

		friend auto on_mousewheel(wheel w)
		-> decltype(m_on_mousewheel[w])&;
	};

	inline auto on_key_pressed(key k)
	-> decltype(input::get().m_on_key_pressed[k])&
	{
		if(!input::get().is_key_valid(k))
			throw std::runtime_error{"invalid key passed"};
		return input::get().m_on_key_pressed[k];
	}

	template<typename... Keys>
	auto on_keys_pressed(Keys&&... keys)
	-> decltype(input::get().m_on_keys_pressed[key_vec{}])&
	{
		key_vec keys_vec;
		mlk::cnt::make_vector(keys_vec, std::forward<Keys>(keys)...);
		return input::get().m_on_keys_pressed[keys_vec];
	}

	inline bool is_key_pressed(key k)
	{return input::get().m_key_bits & k;}

	inline void simulate_keypress(key k)
	{
		input::get().key_pressed(k);
		input::get().key_released(k);
	}

	inline auto on_btn_pressed(btn b)
	-> decltype(input::get().m_on_btn_pressed[b])&
	{return input::get().m_on_btn_pressed[b];}

	inline bool is_btn_pressed(btn b)
	{return input::get().m_mousebtn_bits & b;}

	inline void simulate_btnpress(btn b)
	{
		input::get().btn_pressed(b);
		input::get().btn_released(b);
	}

	inline auto on_mousewheel(wheel w)
	-> decltype(input::get().m_on_mousewheel[w])&
	{return input::get().m_on_mousewheel[w];}
}


#endif // RJ_SHARED_INPUT_HPP
