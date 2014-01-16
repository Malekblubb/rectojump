//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_CORE_MAIN_MENU_MAIN_MENU_HPP
#define RJ_CORE_MAIN_MENU_MAIN_MENU_HPP


#include "component_manager.hpp"
#include "items.hpp"
#include "menu_levels.hpp"
#include "menu_start.hpp"
#include "submenu_manager.hpp"
#include "title.hpp"
#include <rectojump/core/game_window.hpp>
#include <rectojump/core/render.hpp>
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
	class game_handler;

	class main_menu
	{
		friend class component_manager<main_menu>;

		game& m_game;
		game_window& m_gamewindow;
		data_manager& m_datamgr;
		level_manager& m_lvmgr;

		sf::Font m_font{m_datamgr.get_as<sf::Font>("Fipps-Regular.otf")};
		const vec2f m_center{static_cast<vec2f>(m_gamewindow.get_size()) / 2.f};
		const sf::Color m_def_fontcolor{to_rgb("#797979") /*"#797979"_rgb*/}; // TODO: QTC dont supports that custom literals yet
		const sf::Color m_act_fontcolor{to_rgb("#f15ede") /*"#f15ede"_rgb*/};

		// background
		sf::RectangleShape m_background;
		sf::Texture m_background_texture{m_datamgr.get_as<sf::Texture>("menu_side.png")};

		// components (menus)
		component_manager<main_menu> m_componentmgr{*this};
		comp_ptr<menu_start<main_menu>> m_start{m_componentmgr.create_comp<menu_start<main_menu>, menu_state::menu_start>()};
		comp_ptr<menu_levels<main_menu>> m_levels{m_componentmgr.create_comp<menu_levels<main_menu>, menu_state::menu_levels>()};

		// other components (not menus)
		title m_title{m_game, m_font, m_center};

		// menu state
		submenu_manager<menu_state, menu_state::menu_start> m_submenumgr;
		mlk::event_delegates<menu_state> m_on_menu_switch;

	public:
		main_menu(game& g, game_window& gw, data_manager& dm, level_manager& lvmgr) :
			m_game{g},
			m_gamewindow{gw},
			m_datamgr{dm},
			m_lvmgr{lvmgr}
		{this->init();}

		void update(dur duration)
		{
			m_submenumgr.update_current_state(duration);
			m_title.update(duration);
		}

		void render()
		{
			render::render_object(m_game, m_background); // render bg first !!
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

		game& get_game() noexcept
		{return m_game;}

		game_window& get_gamewindow() noexcept
		{return m_gamewindow;}

		data_manager& get_datamgr() noexcept
		{return m_datamgr;}

		level_manager& get_lvmgr() noexcept
		{return m_lvmgr;}

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
		}

		void setup_interface()
		{
			// background
			m_background.setSize(vec2f{m_gamewindow.get_size()});
			m_background.setPosition({0.f, 0.f});
			m_background.setTexture(&m_background_texture);
		}
	};
}


#endif // RJ_CORE_MAIN_MENU_MAIN_MENU_HPP
