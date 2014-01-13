//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_CORE_MAIN_MENU_MENU_LEVELS_HPP
#define RJ_CORE_MAIN_MENU_MENU_LEVELS_HPP


#include "level_squares.hpp"
#include "menu_component.hpp"
#include <rectojump/core/game_window.hpp>
#include <rectojump/shared/level_manager/level_manager.hpp>
#include <rectojump/shared/utils.hpp>


namespace rj
{
	template<typename Main_Menu>
	class menu_levels : public menu_component<Main_Menu>
	{
		level_manager& m_lvmgr{this->m_mainmenu.get_lvmgr()};
		game_window& m_gamewindow{this->m_mainmenu.get_gamewindow()};
		sf::RectangleShape m_bg_top{{200, 200}};

		level_squares m_squares;
		std::size_t m_current_index{0};

	public:
		menu_levels(Main_Menu& mm, menu_state type, game& g, const sf::Font& font, const vec2f& center) :
			menu_component<Main_Menu>{mm, type, g, font, center},
			m_squares{g, font, center, this->m_mainmenu.get_def_fontcolor(),
					  this->m_mainmenu.get_act_fontcolor()}
		{this->init();}

		void update(dur duration) override
		{
			m_squares.update(duration);
		}

		void render() override
		{
			m_squares.render();
			render::render_object(this->m_game, m_bg_top);
		}

		level_squares& get_squares() noexcept
		{return m_squares;}

	private:
		void init()
		{
			m_bg_top.setOrigin(100, 0);
			m_bg_top.setFillColor(to_rgb("#e3e3e3"));
			m_bg_top.setPosition(this->m_center.x, 0.f);
		}
	};
}


#endif // RJ_CORE_MAIN_MENU_MENU_LEVELS_HPP
