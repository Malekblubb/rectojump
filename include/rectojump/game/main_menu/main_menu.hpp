//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_CORE_MAIN_MENU_MAIN_MENU_HPP
#define RJ_CORE_MAIN_MENU_MAIN_MENU_HPP


#include "background.hpp"
#include "component_manager.hpp"
#include "items.hpp"
#include "menu_levels.hpp"
#include "menu_start.hpp"
#include "submenu_manager.hpp"
#include "title.hpp"
#include <rectojump/core/game_window.hpp>
#include <rectojump/core/render.hpp>
#include <rectojump/game/background/background_manager.hpp>
#include <rectojump/game/components/player.hpp>
#include <rectojump/game/factory.hpp>
#include <rectojump/global/common.hpp>
#include <rectojump/shared/level_manager/level_manager.hpp>
#include <rectojump/shared/data_manager.hpp>
#include <rectojump/shared/utils.hpp>

#include <SFML/Graphics.hpp>


namespace rj
{
	class game;

	template<typename Game_Handler>
	class main_menu
	{
		friend class component_manager<main_menu<Game_Handler>>;

		Game_Handler& m_gamehandler;
		game_window& m_gamewindow;
		game& m_game;
		data_manager& m_datamgr;
		level_manager& m_lvmgr;
		background_manager& m_backgroundmgr;

		sf::Font m_font{m_datamgr.get_as<sf::Font>("Fipps-Regular.otf")};
		const vec2f m_center{static_cast<vec2f>(m_gamewindow.get_size()) / 2.f};
		const sf::Color m_def_fontcolor{to_rgb("#797979") /*"#797979"_rgb*/}; // TODO: QTC dont supports that custom literals yet
		const sf::Color m_act_fontcolor{to_rgb("#f15ede") /*"#f15ede"_rgb*/};

		// background
		background m_background{m_game, m_datamgr, m_backgroundmgr};

		// components (menus)
		component_manager<main_menu> m_componentmgr{*this};
		comp_ptr<menu_start<main_menu<Game_Handler>>> m_start{m_componentmgr.template create_comp<menu_start<main_menu<Game_Handler>>, menu_state::menu_start>()};
		comp_ptr<menu_levels<main_menu<Game_Handler>>> m_levels{m_componentmgr.template create_comp<menu_levels<main_menu<Game_Handler>>, menu_state::menu_levels>()};

		// other components (not menus)
		title m_title{m_game, m_font, m_center};

		// menu state
		submenu_manager<menu_state, menu_state::menu_start> m_submenumgr;
		mlk::event_delegates<menu_state> m_on_menu_switch;

	public:
		main_menu(Game_Handler& gh/* game_window& gw, game& g, data_manager& dm, level_manager& lm, background_manager& bgm*/) :
			m_gamehandler{gh},
			m_gamewindow{gh.get_gamewindow()},
			m_game{gh.get_game()},
			m_datamgr{gh.get_datamgr()},
			m_lvmgr{gh.get_levelmgr()},
			m_backgroundmgr{gh.get_backgroundmgr()}
		{this->init();}

		void update(dur duration)
		{
			m_submenumgr.update_current_state(duration);
			m_background.update(duration);
			m_title.update(duration);
		}

		void render()
		{
			m_background.render();
			m_submenumgr.render_current_state();
			m_title.render();
		}

		void on_key_up()
		{m_submenumgr.event_up();}

		void on_key_down()
		{m_submenumgr.event_down();}

		void on_key_backspace()
		{
			// activate prev state from submenu
			auto ptr(m_componentmgr.get_comp_from_type(m_submenumgr.get_current_state()));
			if(ptr != nullptr)
				if(ptr->is_accessing_submenu())
				{
					ptr->on_key_backspace();
					return;
				}

			// activate prev state
			this->do_menu_switch_back();
		}

		bool is_active(menu_state s) const noexcept
		{return m_submenumgr.get_current_state() == s;}

		void call_current_itemevent()
		{m_submenumgr.event_current();}

		void do_menu_switch(menu_state s)
		{
			m_submenumgr.switch_state(s);
			m_on_menu_switch[s]();
		}

		void do_menu_switch_back()
		{
			m_submenumgr.activate_prev_state();
			m_on_menu_switch[m_submenumgr.get_current_state()]();
		}

		auto get_gamehandler()
		-> decltype(m_gamehandler)&
		{return m_gamehandler;}

		const sf::Color& get_act_fontcolor() const noexcept
		{return m_act_fontcolor;}

		const sf::Color& get_def_fontcolor() const noexcept
		{return m_def_fontcolor;}

	private:
		void init()
		{
			m_submenumgr.add_menu(menu_state::menu_start, m_start);
			m_submenumgr.add_menu(menu_state::menu_levels, m_levels);

			this->setup_events();
			this->setup_interface();
		}

		void setup_events()
		{
			m_on_menu_switch[menu_state::menu_levels] +=
			[this]{m_title.set_text("Levels");};

			m_on_menu_switch[menu_state::menu_start] +=
			[this]{m_title.set_text("Recto Jump");};


			m_start->get_items().on_event("play",
			[this]{this->do_menu_switch(menu_state::menu_levels);});

			m_start->get_items().on_event("quit",
			[this]{m_gamewindow.stop();});


			m_levels->get_items().on_event("lv_download",
			[this]
			{
				m_gamehandler.create_popup("Not available yet.", vec2f{settings::get_window_size<vec2f>().x / 2.f,
																  settings::get_window_size<vec2f>().y - 100.f});
			});
		}

		void setup_interface()
		{

		}
	};
}


#endif // RJ_CORE_MAIN_MENU_MAIN_MENU_HPP
