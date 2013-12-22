//
// Copyright (c) 2013 Christoph Malek
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
		mlk::uint m_width{800}, m_height{600};
		sf::RenderWindow m_window;
		bool m_running{false};

		input m_input;

		game_updater m_game_updater;

	public:
		mlk::slot<> on_stop;

		game_window() :
			m_window{{m_width, m_height}, "RectoJump"}
		{m_input.on_key_pressed[key::Escape] += [this]{this->stop();};}

		// interface
		void start()
		{
			if(m_running) return;

			this->prepare_start();
			while(m_running)
			{
				m_game_updater.start_pt();

				// update
				this->update_events();
				m_game_updater.update();

				// render
				m_window.clear();
				m_game_updater.render();
				m_window.display();

				m_game_updater.end_pt();
			}
		}

		void stop() noexcept
		{m_running = false; on_stop();}

		void draw(const sf::Drawable& object)
		{m_window.draw(object);}

		// getters
		game_updater& updater() noexcept
		{return m_game_updater;}

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
				case sf::Event::EventType::MouseButtonPressed:
					m_input.btn_pressed(ev.mouseButton.button);
					break;
				default: break;
				}
			}
		}
	};
}


#endif // RJ_CORE_GAME_WINDOW_HPP
