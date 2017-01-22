//
// Copyright (c) 2013-2017 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_SHARED_INPUT_HPP
#define RJ_SHARED_INPUT_HPP


#include <rectojump/global/common.hpp>
#include <rectojump/global/errors.hpp>

#include <mlk/containers/container_utl.h>
#include <mlk/log/log.h>
#include <mlk/signals_slots/slot.h>
#include <mlk/tools/bitset.h>


namespace rj
{
	// forward helper funcs
	namespace inp
	{
		mlk::slot<>& on_input_update();
		char get_last_textinput();
		mlk::slot<>& on_key_pressed(key k);
		template<typename... Keys> mlk::slot<>& on_keys_pressed(Keys&&...);
		bool is_key_pressed(key);
		void simulate_keypress(key);
		mlk::slot<const vec2f&>& on_btn_pressed(btn b);
		bool is_btn_pressed(btn);
		bool was_real_mousepress_left();
		void simulate_btnpress(btn);
		mlk::slot<const vec2f&>& on_mousewheel(wheel w);
		vec2f& get_mousepos();
		vec2f& get_lastmousepos();
		vec2f get_mousepos_current_view();
	}

	using key_vec = std::vector<key>;

	class game_window;

	template<typename Game_Window>
	class input
	{
		friend class game_window;
		Game_Window* m_gamewindow;
		sf::RenderWindow* m_renderwindow;

		mlk::bitset<btn, btn::ButtonCount> m_mousebtn_bits;
		mlk::bitset<key, key::KeyCount> m_key_bits;

		mlk::event_delegates<key> m_on_key_pressed;
		mlk::event_delegates<key_vec> m_on_keys_pressed;
		mlk::event_delegates<btn, const vec2f&> m_on_btn_pressed;
		mlk::event_delegates<wheel, const vec2f&> m_on_mousewheel;
		char m_last_textinput;
		bool m_real_mousepress_left{false};

		vec2f m_mousepos{0.f, 0.f};
		vec2f m_last_mousepos{0.f, 0.f};

		mlk::slot<> m_on_update;

	public:
		input() = default;

		static input& get() noexcept
		{static input i; return i;}

		bool is_key_valid(key k) const noexcept
		{return k != key::Unknown;}

		void reset_last_states() noexcept
		{
			m_last_textinput = 0;
			m_real_mousepress_left = false;
		}

	private:
		void init(Game_Window* gw) noexcept
		{
			m_gamewindow = gw;
			m_renderwindow = &gw->get_renderwindow();

			if(m_gamewindow == nullptr || m_renderwindow == nullptr)
				mlk::lerr(errors::cl_nullptr_access)["rj::input<T>"];

			m_gamewindow->on_event(sf::Event::EventType::TextEntered) +=
			[this](sf::Event e){m_last_textinput = e.text.unicode;};
		}

		void update()
		{
			m_last_mousepos = m_mousepos;
			m_mousepos = m_renderwindow->mapPixelToCoords(sf::Mouse::getPosition(*m_renderwindow));
			m_on_update();
		}

		void key_pressed(key k)
		{
			if(!this->is_key_valid(k))
				return;

			if(mlk::cnt::exists_map_first(k, m_on_key_pressed))
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
		{
			m_mousebtn_bits |= b;

			if(mlk::cnt::exists_map_first(b, m_on_btn_pressed))
				m_on_btn_pressed[b](m_mousepos);
		}

		void btn_released(btn b)
		{
			if((b == btn::Left) && (m_mousebtn_bits & b))
				m_real_mousepress_left = true;
			m_mousebtn_bits.remove(b);
		}

		void mousewheel_moved(int delta)
		{delta < 0 ? m_on_mousewheel[wheel::down](m_mousepos) : m_on_mousewheel[wheel::up](m_mousepos);}


		friend mlk::slot<>& inp::on_input_update();
		friend char inp::get_last_textinput();
		friend mlk::slot<>& inp::on_key_pressed(key k);
		template<typename... Keys>
		friend mlk::slot<>& inp::on_keys_pressed(Keys&&...);
		friend bool inp::is_key_pressed(key);
		friend void inp::simulate_keypress(key);
		friend mlk::slot<const vec2f&>& inp::on_btn_pressed(btn b);
		friend bool inp::is_btn_pressed(btn);
		friend bool inp::was_real_mousepress_left();
		friend void inp::simulate_btnpress(btn);
		friend mlk::slot<const vec2f&>& inp::on_mousewheel(wheel w);
		friend vec2f& inp::get_mousepos();
		friend vec2f& inp::get_lastmousepos();
		friend vec2f inp::get_mousepos_current_view();
	};

	namespace inp
	{
		// get a instance of input<game_window>
		inline auto& i()
		{return input<game_window>::get();}

		inline mlk::slot<>& on_input_update()
		{return input<game_window>::get().m_on_update;}

		inline char get_last_textinput()
		{return input<game_window>::get().m_last_textinput;}

		inline mlk::slot<>& on_key_pressed(key k)
		{
			if(!input<game_window>::get().is_key_valid(k))
				throw std::runtime_error{"invalid key passed"};
			return input<game_window>::get().m_on_key_pressed[k];
		}

		template<typename... Keys>
		mlk::slot<>& on_keys_pressed(Keys&&... keys)
		{
			key_vec keys_vec{std::forward<Keys>(keys)...};
			return input<game_window>::get().m_on_keys_pressed[keys_vec];
		}

		inline bool is_key_pressed(key k)
		{return input<game_window>::get().m_key_bits & k;}

		inline bool was_real_mousepress_left()
		{return input<game_window>::get().m_real_mousepress_left;}

		inline void simulate_keypress(key k)
		{
			input<game_window>::get().key_pressed(k);
			input<game_window>::get().key_released(k);
		}

		inline mlk::slot<const vec2f&>& on_btn_pressed(btn b)
		{return input<game_window>::get().m_on_btn_pressed[b];}

		inline bool is_btn_pressed(btn b)
		{return input<game_window>::get().m_mousebtn_bits & b;}

		inline void simulate_btnpress(btn b)
		{
			input<game_window>::get().btn_pressed(b);
			input<game_window>::get().btn_released(b);
		}

		inline mlk::slot<const vec2f&>& on_mousewheel(wheel w)
		{return input<game_window>::get().m_on_mousewheel[w];}

		inline vec2f& get_mousepos()
		{return input<game_window>::get().m_mousepos;}

		inline vec2f& get_lastmousepos()
		{return input<game_window>::get().m_last_mousepos;}

		inline vec2f get_mousepos_current_view()
		{
			auto& rw(input<game_window>::get().m_renderwindow);
			return rw->mapPixelToCoords(sf::Mouse::getPosition(*rw), rw->getView());
		}

		inline vec2f get_mousepos(const sf::RenderWindow& rw, const sf::View& v)
		{return rw.mapPixelToCoords(sf::Mouse::getPosition(rw), v);}

		template<bool current_view = false>
		sf::FloatRect get_mousebounds()
		{
			auto& pos(get_mousepos());
			return {pos.x, pos.y, 1.f, 1.f};
		}

		template<>
		inline sf::FloatRect get_mousebounds<true>()
		{
			auto pos(get_mousepos_current_view());
			return {pos.x, pos.y, 1.f, 1.f};
		}
	}
}


#endif // RJ_SHARED_INPUT_HPP
