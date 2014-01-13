//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_CORE_MAIN_MENU_MENU_START_HPP
#define RJ_CORE_MAIN_MENU_MENU_START_HPP


#include "items.hpp"
#include "menu_component.hpp"
#include <rectojump/game/components/player.hpp>
#include <rectojump/game/factory.hpp>

#include <mlk/time/simple_timer.h>
#include <mlk/tools/random_utl.h>
#include <mlk/types/types.h>


namespace rj
{
	template<typename Main_Menu>
	class menu_start : public menu_component<Main_Menu>
	{
		items m_items;

		// player preview
		factory::eptr<player> m_player_prev{factory::create<player>(vec2f{basic_component::m_center.x, basic_component::m_center.y / 0.55f})};
		mlk::tm::simple_timer m_player_timer{500};

	public:
		menu_start(Main_Menu& mm, menu_state type, game& g, const sf::Font& font, const vec2f& center) :
			menu_component<Main_Menu>{mm, type, g, font, center},
			m_items{g, font, center,this->m_mainmenu.get_def_fontcolor(),
					this->m_mainmenu.get_act_fontcolor()}
		{this->init();}

		items& get_items() noexcept
		{return m_items;}

		item get_current_selected() const noexcept
		{return m_items.get_current_selected();}

		void update(dur duration) override
		{
			m_items.update(duration);

			// update player
			if(m_player_timer.timed_out())
			{
				simulate_keypress(key::Space);
				m_player_timer.restart(mlk::rnd<mlk::ullong>(500, 5000));
			}
			m_player_prev->update(duration);
		}

		void render() override
		{
			m_items.render();
			m_player_prev->render();
		}

	private:
		void init()
		{
			m_items.add_item("play", "Play");
			m_items.add_item("options", "Options");
			m_items.add_item("credits", "Credits");
			m_items.add_item("quit", "Quit");

			m_player_prev->init();
			m_player_prev->set_game(&this->m_game);
			m_player_prev->render_object().setFillColor(this->m_mainmenu.get_act_fontcolor());
			m_player_timer.run();
		}
	};
}



#endif // RJ_CORE_MAIN_MENU_MENU_START_HPP
