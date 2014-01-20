//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_CORE_MAIN_MENU_MENU_LEVELS_HPP
#define RJ_CORE_MAIN_MENU_MENU_LEVELS_HPP


#include "items.hpp"
#include "level_squares.hpp"
#include "menu_component.hpp"
#include "submenu_manager.hpp"
#include <rectojump/core/game_window.hpp>
#include <rectojump/shared/level_manager/level_manager.hpp>
#include <rectojump/shared/utils.hpp>


namespace rj
{
	enum class lv_menu_state : char
	{select, local, download};

	template<typename Main_Menu>
	class menu_levels : public menu_component<Main_Menu>
	{
		level_manager& m_lvmgr{this->m_mainmenu.get_gamehandler().get_levelmgr()};
		game_window& m_gamewindow{this->m_mainmenu.get_gamehandler().get_gamewindow()};
		sf::RectangleShape m_bg_top{{200, 200}};

		mlk::sptr<level_squares> m_squares_local;
		mlk::sptr<items> m_items;
		submenu_manager<lv_menu_state, lv_menu_state::select> m_submenu_manager;

	public:
		mlk::slot<const level_id&>& on_level_load{m_squares_local->on_level_load};

		menu_levels(Main_Menu& mm, menu_state type, game& g, const sf::Font& font, const vec2f& center) :
			menu_component<Main_Menu>{mm, type, g, font, center},
			m_squares_local{std::make_shared<level_squares>(g, font, center, this->m_mainmenu.get_def_fontcolor(),
															this->m_mainmenu.get_act_fontcolor())},
			m_items{std::make_shared<items>(g, font, center, this->m_mainmenu.get_def_fontcolor(),
					this->m_mainmenu.get_act_fontcolor())}
		{this->init();}

		void update(dur duration) override
		{m_submenu_manager.update_current_state(duration);}

		void render() override
		{
			m_submenu_manager.render_current_state();
			rndr::ro(this->m_game, m_bg_top);
		}

		void on_key_up() override
		{m_submenu_manager.event_up();}

		void on_key_down() override
		{m_submenu_manager.event_down();}

		void call_current_event() override
		{m_submenu_manager.event_current();}

		void on_key_backspace() override
		{m_submenu_manager.activate_prev_state();}

		bool is_accessing_submenu() override
		{return m_submenu_manager.is_submenu_active();}

		items& get_items() noexcept
		{return *m_items;}

	private:
		void init()
		{
			// adding components/submenus
			m_submenu_manager.add_menu(lv_menu_state::select, m_items);
			m_submenu_manager.add_menu(lv_menu_state::local, m_squares_local);

			// add entrys to menu 'items'
			m_items->add_item("lv_local", "Local Levels");
			m_items->add_item("lv_download", "Download Levels");

			// bind events
			m_items->on_event("lv_local", [this]{m_submenu_manager.switch_state(lv_menu_state::local);});

			m_bg_top.setOrigin(100, 0);
			m_bg_top.setFillColor(to_rgb("#e3e3e3"));
			m_bg_top.setPosition(this->m_center.x, 0.f);
		}
	};
}


#endif // RJ_CORE_MAIN_MENU_MENU_LEVELS_HPP
