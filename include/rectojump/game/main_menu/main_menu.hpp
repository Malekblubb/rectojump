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
#include "title.hpp"

#include <rectojump/core/game_window.hpp>
#include <rectojump/core/render.hpp>
#include <rectojump/game/components/player.hpp>
#include <rectojump/game/factory.hpp>
#include <rectojump/global/common.hpp>
#include <rectojump/shared/level_manager/level_manager.hpp>
#include <rectojump/shared/data_manager.hpp>
#include <rectojump/shared/utils.hpp>

#include <mlk/signals_slots/slot.h>
#include <mlk/tools/bitset.h>
#include <mlk/tools/enum_utl.h>

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

		mlk::event_delegates<item_id> m_events;

		// menu components
		component_manager<main_menu> m_componentmgr{*this};
		comp_ptr<menu_start<main_menu>> m_start{m_componentmgr.create_comp<menu_start<main_menu>, menu_state::menu_start>()};
		comp_ptr<menu_levels<main_menu>> m_levels{m_componentmgr.create_comp<menu_levels<main_menu>, menu_state::menu_levels>()};
		comp_ptr<title<main_menu>> m_title{m_componentmgr.create_comp<title<main_menu>, menu_state::title>()};

		// menu states
		mlk::ebitset<menu_state, menu_state::num> m_current_menu;

	public:
		main_menu(game& g, game_window& gw, data_manager& dm, level_manager& lvmgr) :
			m_game{g},
			m_gamewindow{gw},
			m_datamgr{dm},
			m_lvmgr{lvmgr}
		{this->init();}

		bool is_active(menu_state s)
		{return m_current_menu & s;}

		auto on_item_event(const item_id& id)
		-> decltype(m_events[id])&
		{return m_events[id];}

		void exec_current_itemevent()
		{m_events[m_start->get_current_selected().id]();}

		template<menu_state new_state>
		void do_menu_switch()
		{
			m_current_menu.remove_all();
			m_current_menu |= new_state;
			m_current_menu |= menu_state::title;
		}

		void update(dur duration)
		{
			m_componentmgr.update(duration);
		}

		void render()
		{
			render::render_object(m_game, m_background);
			m_componentmgr.render();
		}

		// getters
		items& get_items() noexcept
		{return m_start->get_items();}

		level_squares& get_squares() noexcept
		{return m_levels->get_squares();}

		game_window& get_gamewindow() noexcept
		{return m_gamewindow;}

		data_manager& get_datamgr() const noexcept
		{return m_datamgr;}

		level_manager& get_lvmgr() const noexcept
		{return m_lvmgr;}

		const sf::Color& get_act_fontcolor() const noexcept
		{return m_act_fontcolor;}

		const sf::Color& get_def_fontcolor() const noexcept
		{return m_def_fontcolor;}

	private:
		void init()
		{
			m_current_menu |= menu_state::menu_start;
			m_current_menu |= menu_state::title;

			this->setup_events();
			this->setup_interface();
		}

		void setup_events()
		{
			this->on_item_event("play") +=
			[this]
			{
				this->do_menu_switch<menu_state::menu_levels>();
				m_title->set_text("Levels");
			};
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