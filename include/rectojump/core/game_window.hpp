//
// Copyright (c) 2013-2014 Christoph Malek
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
	class game_window
	{
		mlk::uint m_width{1280}, m_height{720};
		std::string m_title{"Recto Jump"};
		sf::RenderWindow m_window;
		bool m_running{false};
		bool m_fullscreen{false};
		bool m_need_recreate{false};

		input& m_input{input::get()};

		game_updater m_game_updater;

	public:
		mlk::slot<> on_stop;

		game_window() :
			m_window{{m_width, m_height}, m_title}
		{ }

		// interface
		void start()
		{
			if(m_running) return;

			this->prepare_start();
			while(m_running)
			{
				if(m_need_recreate)
					this->recreate();

				m_game_updater.start_pt();

				// update
				this->update_events();
				m_input.update(m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window)));
				m_game_updater.update();

				// render
				m_window.clear();
				m_game_updater.render();
				m_window.display();

				m_game_updater.end_pt();
			}
		}

		void stop() noexcept
		{on_stop(); m_running = false;}

		void draw(const sf::Drawable& object)
		{m_window.draw(object);}

		void toggle_fullscreen() noexcept
		{
			m_fullscreen = !m_fullscreen;
			m_need_recreate = true;
		}

		// setters
		void set_framereate_limit(mlk::uint limit) noexcept
		{m_window.setFramerateLimit(limit);}

		void set_size(const vec2u& size) noexcept
		{m_window.setSize(size);}

		void set_position(const vec2i& position) noexcept
		{m_window.setPosition(position);}

		void set_fullscreen(bool b) noexcept
		{
			if(b == m_fullscreen)
				return;
			m_fullscreen = b;
			m_need_recreate = true;
		}

		void set_view(const sf::View& v) noexcept
		{m_window.setView(v);}

		// getters
		game_updater& get_updater() noexcept
		{return m_game_updater;}

		vec2u get_size() const noexcept
		{return m_window.getSize();}

		vec2i get_position() const noexcept
		{return m_window.getPosition();}

		bool get_fullscreen() const noexcept
		{return m_fullscreen;}

	private:
		void prepare_start() noexcept
		{
			m_running = true;
			mlk::lout("rj::game_window", true) << "starting gamewindow";
		}

		void update_events() noexcept
		{
			sf::Event ev;
			while(m_window.pollEvent(ev))
			{
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
					m_input.mousewheel_moved(ev.mouseWheel.delta,
											 m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window)));
					break;
				default: break;
				}
			}
		}

		void recreate()
		{
			m_window.close();
			m_window.create({m_width, m_height}, m_title, m_fullscreen ? sf::Style::Fullscreen : sf::Style::Default);
			m_need_recreate = false;
		}
	};
}


#endif // RJ_CORE_GAME_WINDOW_HPP
