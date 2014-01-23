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
	template<typename Game_Handler, typename Game>
	class debug_info
	{
		Game_Handler& m_gamehandler;
		Game& m_game;

		sf::RectangleShape m_background{{100.f, 100.f}};
		debug_text m_text;

	public:
		debug_info(Game_Handler& gh) :
			m_gamehandler{gh},
			m_game{gh.get_game()},
			m_text{gh.get_datamgr()}
		{
			m_background.setPosition({1.f, 1.f});
			m_background.setFillColor({255, 150, 123, 220});
			m_background.setOutlineThickness(1);
			m_background.setOutlineColor({255, 0, 0});
		}

		void update(dur)
		{
			m_background.setSize(m_text.get_size());
			auto text(mlk::stl_string::str_format(
						  "Performance\n===========\nFPS:        %%\nFDur:       %%\n\nComponents\n==========\nGameworld:  %%\nBackground: %%\nPopups:     %%\n\nManagers\n========\nData:       %%\nLevels:     %%",
						  m_gamehandler.get_gamewindow().get_updater().get_fps(),
						  m_gamehandler.get_gamewindow().get_updater().get_frameduration(),
						  m_game.get_world().num_entities(),
						  m_gamehandler.get_backgroundmgr().num_components(),
						  m_gamehandler.get_popupmgr().num_popups(),
						  m_gamehandler.get_datamgr().num_data(),
						  m_gamehandler.get_levelmgr().num_levels()));
			m_text.set_text(text);
		}

		void render()
		{rndr::rmo(m_game, m_background, m_text);}
	};
}


#endif // RJ_GAME_DEBUG_INFO_HPP
