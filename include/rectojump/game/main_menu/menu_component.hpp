//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_CORE_MAIN_MENU_MENU_COMPONENT_HPP
#define RJ_CORE_MAIN_MENU_MENU_COMPONENT_HPP


#include <rectojump/global/common.hpp>

#include <SFML/Graphics.hpp>


namespace rj
{
	class game;

	class menu_component
	{
	protected:
		game& m_game;
		const sf::Font& m_font;
		const vec2f& m_center;

	public:
		menu_component(game& g, const sf::Font& font, const vec2f& center) :
			m_game{g},
			m_font{font},
			m_center{center}
		{ }

		virtual void init() { }
		virtual void update(dur) = 0;
		virtual void render() = 0;
	};
}


#endif // RJ_CORE_MAIN_MENU_MENU_COMPONENT_HPP
