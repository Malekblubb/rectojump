//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_DEBUG_INFO_HPP
#define RJ_GAME_DEBUG_INFO_HPP


#include <rectojump/core/render.hpp>
#include <rectojump/global/common.hpp>
#include <rectojump/shared/debug_text.hpp>

#include <mlk/tools/stl_string_utl.h>


namespace rj
{
	class game;
	class game_handler;

	class debug_info
	{
		game& m_game;
		sf::RectangleShape m_background{{100.f, 100.f}};
		debug_text m_text;

	public:
		debug_info(game& g, data_manager& dm) :
			m_game{g},
			m_text{dm}
		{
			m_background.setPosition({1.f, 1.f});
			m_background.setFillColor({255, 150, 123, 220});
			m_background.setOutlineThickness(1);
			m_background.setOutlineColor({255, 0, 0});
		}

		void update(dur);
		void render()
		{
			rndr::ro(m_game, m_background);
			rndr::ro(m_game, m_text);
		}
	};
}


#endif // RJ_GAME_DEBUG_INFO_HPP
