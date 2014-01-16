//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_CORE_MAIN_MENU_MENU_COMPONENT_HPP
#define RJ_CORE_MAIN_MENU_MENU_COMPONENT_HPP


#include "basic_component.hpp"
#include "items.hpp"
#include "main_menu.hpp"


namespace rj
{
	class game;

	enum class menu_state : char
	{none, menu_start, menu_levels, num};

	template<typename Main_Menu>
	class menu_component : public basic_component
	{
	protected:
		Main_Menu& m_mainmenu;
		const menu_state m_type;

	public:
		menu_component(Main_Menu& mm, menu_state state, game& g, const sf::Font& font, const vec2f& center) :
			basic_component{g, font, center},
			m_mainmenu{mm},
			m_type{state}
		{ }

		virtual void update(dur) = 0;
		virtual void render() = 0;
		virtual void on_key_up() = 0;
		virtual void on_key_down() = 0;
		virtual void call_current_event() = 0;
		virtual void on_key_backspace() { }
		virtual bool is_accessing_submenu() {return false;}

		menu_state get_type() const noexcept
		{return m_type;}
	};
}


#endif // RJ_CORE_MAIN_MENU_MENU_COMPONENT_HPP
