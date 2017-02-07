//
// Copyright (c) 2013-2017 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_CORE_GAME_WINDOW_HPP
#define RJ_CORE_GAME_WINDOW_HPP

#include "game_updater.hpp"
#include <rectojump/shared/input.hpp>

#include <mlk/log/log.h>
#include <mlk/signals_slots/slot.h>
#include <mlk/time/time.h>
#include <mlk/types/types.h>

#include <SFML/Graphics.hpp>

namespace rj
{
	using window_style = int;

	class game_window
	{
		mlk::uint m_width, m_height;
		std::string m_title{"Recto Jump"};
		sf::RenderWindow m_window;
		window_style m_windowstyles;
		bool m_running{false};
		bool m_need_recreate{false};

		game_updater m_game_updater;
		input<game_window>& m_input{input<game_window>::get()};

		mlk::event_delegates<sf::Event::EventType, sf::Event> m_on_event;

	public:
		mlk::slot<> on_stop;

		game_window(const vec2u& size, bool fullscreen = false)
			: m_width{size.x}, m_height{size.y}
		{
			this->set_fullscreen(fullscreen);
			this->init();
		}

		// interface
		void start()
		{
			if(m_running) return;

			this->prepare_start();
			while(m_running) {
				if(m_need_recreate) this->recreate();

				m_game_updater.start_pt();

				// update
				this->update_events();
				m_input.update();
				m_game_updater.update();

				// render
				m_window.clear();
				m_game_updater.render();
				m_window.display();

				m_game_updater.end_pt();
			}
		}

		void stop() noexcept
		{
			on_stop();
			m_running = false;
		}

		auto& on_event(sf::Event::EventType type) { return m_on_event[type]; }

		template <typename... Args>
		void draw(Args&&... args)
		{
			m_window.draw(std::forward<Args>(args)...);
		}

		void toggle_fullscreen() noexcept
		{
			m_windowstyles ^= sf::Style::Fullscreen;
			m_need_recreate = true;
		}

		void toggle_titlebar() noexcept
		{
			m_windowstyles ^= sf::Style::Titlebar;
			m_need_recreate = true;
		}

		// setters
		void set_framereate_limit(mlk::uint limit) noexcept
		{
			m_window.setFramerateLimit(limit);
		}

		void set_size(const vec2u& size) noexcept { m_window.setSize(size); }

		void set_position(const vec2i& position) noexcept
		{
			m_window.setPosition(position);
		}

		void set_fullscreen(bool b) noexcept
		{
			if(b == (m_windowstyles & sf::Style::Fullscreen)) return;
			b ? m_windowstyles |= sf::Style::Fullscreen
			  : m_windowstyles &= ~sf::Style::Fullscreen;
			m_need_recreate = true;
		}

		void set_titlebar(bool b) noexcept
		{
			if(b == (m_windowstyles & sf::Style::Titlebar)) return;
			b ? m_windowstyles |= sf::Style::Titlebar
			  : m_windowstyles &= ~sf::Style::Titlebar;
			m_need_recreate = true;
		}

		void set_view(const sf::View& v) noexcept { m_window.setView(v); }

		// getters
		sf::RenderWindow& get_renderwindow() noexcept { return m_window; }

		game_updater& get_updater() noexcept { return m_game_updater; }

		vec2u get_size() const noexcept { return m_window.getSize(); }

		vec2i get_position() const noexcept { return m_window.getPosition(); }

		bool get_fullscreen() const noexcept
		{
			return m_windowstyles & sf::Style::Fullscreen;
		}

		bool get_titlebar() const noexcept
		{
			return m_windowstyles & sf::Style::Titlebar;
		}

	private:
		void init()
		{
			m_input.init(this);
			m_windowstyles |= sf::Style::Default;
			this->recreate();
		}

		void prepare_start() noexcept
		{
			m_running = true;
			mlk::lout("rj::game_window", true) << "starting gamewindow";
		}

		void update_events() noexcept
		{
			sf::Event ev;
			while(m_window.pollEvent(ev)) {
				switch(ev.type)
				{
					case sf::Event::EventType::Closed:
						this->stop();
						break;
					case sf::Event::EventType::KeyPressed:
						m_input.key_pressed(ev.key.code);
						break;
					case sf::Event::EventType::KeyReleased:
						m_input.key_released(ev.key.code);
						break;
					case sf::Event::EventType::MouseButtonPressed:
						m_input.btn_pressed(ev.mouseButton.button);
						break;
					case sf::Event::EventType::MouseButtonReleased:
						m_input.btn_released(ev.mouseButton.button);
						break;
					case sf::Event::EventType::MouseWheelMoved:
						m_input.mousewheel_moved(ev.mouseWheel.delta);
						break;
					default:
						break;
				}

				// call custom on_event
				m_on_event[ev.type](ev);
			}
		}

		void recreate()
		{
			m_window.close();
			m_window.create({m_width, m_height}, m_title, m_windowstyles);
			m_need_recreate = false;
		}
	};
}

#endif// RJ_CORE_GAME_WINDOW_HPP
