//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_CORE_MAIN_MENU_MAIN_MENU_HPP
#define RJ_CORE_MAIN_MENU_MAIN_MENU_HPP


#include "items.hpp"
#include "title.hpp"

#include <rectojump/core/game_window.hpp>
#include <rectojump/core/render.hpp>
#include <rectojump/game/components/player.hpp>
#include <rectojump/game/factory.hpp>
#include <rectojump/global/common.hpp>
#include <rectojump/shared/data_manager.hpp>
#include <rectojump/shared/utils.hpp>

#include <mlk/signals_slots/slot.h>
#include <mlk/time/simple_timer.h>
#include <mlk/tools/random_utl.h>

#include <SFML/Graphics.hpp>


namespace rj
{
	class game;

	enum item : std::size_t
	{play, options, credits, quit};

	class main_menu
	{
		game& m_game;
		game_window& m_gamewindow;
		data_manager& m_datamanager;

		sf::Font m_font{m_datamanager.get_as<sf::Font>("Fipps-Regular.otf")};
		const vec2f m_center{static_cast<vec2f>(m_gamewindow.get_size()) / 2.f};
		const sf::Color m_def_fontcolor{"#797979"_rgb};
		const sf::Color m_act_fontcolor{"#f15ede"_rgb};

		// background
		sf::RectangleShape m_background;
		sf::Texture m_background_texture{m_datamanager.get_as<sf::Texture>("menu_side.png")};

		// player preview
		factory::eptr<player> m_player_prev{factory::create<player>(vec2f{m_center.x, m_center.y / 0.55f})};
		mlk::tm::simple_timer m_player_timer{500};

		mlk::event_delegates<std::size_t> m_events;

		// menu components
		items m_items{m_game, m_font, m_center, m_def_fontcolor, m_act_fontcolor};
		title m_title{m_game, m_font, m_center};

	public:
		main_menu(game& g, game_window& gw, data_manager& dm) :
			m_game{g},
			m_gamewindow{gw},
			m_datamanager{dm}
		{this->init();}


		void update(dur duration)
		{
			m_title.update(duration);
			m_items.update(duration);
			this->update_player(duration);
		}

		void render()
		{
			render::render_object(m_game, m_background);
			m_title.render();
			m_items.render();
			m_player_prev->render();
		}

	private:
		void init()
		{
			this->setup_input();
			this->setup_events();
			this->setup_interface();
			m_player_timer.run();
		}

		void setup_input()
		{
			//on_keys_pressed(key::Return);
		}

		void setup_events()
		{
			mlk::slot<> test{[]{std::cout<< "test " << std::endl;}};
			m_events.emplace(item::play, test);
		}

		void setup_interface()
		{
			// background
			m_background.setSize(vec2f{m_gamewindow.get_size()});
			m_background.setPosition({0.f, 0.f});
			m_background.setTexture(&m_background_texture);

			// player
			m_player_prev->init();
			m_player_prev->set_game(&m_game);
			m_player_prev->render_object().setFillColor(m_act_fontcolor);
		}

		void update_player(dur duration)
		{
			if(m_player_timer.timed_out())
			{
				simulate_keypress(key::Space);
				m_player_timer.restart(mlk::rnd<mlk::ullong>(500, 5000));
			}
			m_player_prev->update(duration);
		}
	};
}


#endif // RJ_CORE_MAIN_MENU_MAIN_MENU_HPP
