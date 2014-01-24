//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_CORE_MAIN_MENU_BASIC_COMPONENT_HPP
#define RJ_CORE_MAIN_MENU_BASIC_COMPONENT_HPP


#include <rectojump/global/common.hpp>

#include <SFML/Graphics.hpp>


namespace rj
{
	struct basic_component
	{
		rndr& m_render;
		const sf::Font& m_font;
		const vec2f& m_center;

		basic_component(rndr& r, const sf::Font& font, const vec2f& center) :
			m_render{r},
			m_font{font},
			m_center{center}
		{ }

		virtual void update(dur) { }
		virtual void render() { }
		virtual void on_key_up() { }
		virtual void on_key_down() { }
		virtual void on_key_backspace() { }
		virtual void call_current_event() { }
	};
}


#endif // RJ_CORE_MAIN_MENU_BASIC_COMPONENT_HPP
