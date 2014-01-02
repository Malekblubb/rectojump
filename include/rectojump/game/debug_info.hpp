//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_DEBUG_INFO_HPP
#define RJ_GAME_DEBUG_INFO_HPP


#include <rectojump/global/common.hpp>
#include <rectojump/shared/debug_text.hpp>

#include <mlk/tools/stl_string_utl.h>


namespace rj
{
	class game;

	class debug_info
	{
		game& m_game;
		sf::RectangleShape m_background{{100.f, 100.f}};
		debug_text m_text{"debug_font.png"_data};

	public:
		debug_info(game& g) :
			m_game{g}
		{
			m_background.setPosition({1.f, 1.f});
			m_background.setFillColor({255, 150, 123, 100});
			m_background.setOutlineThickness(1);
			m_background.setOutlineColor({255, 0, 0});
		}

		void update(dur);
		void render();
	};
}


#endif // RJ_GAME_DEBUG_INFO_HPP
