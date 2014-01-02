//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_DEBUG_INFO_INL
#define RJ_GAME_DEBUG_INFO_INL


#include "debug_info.hpp"


namespace rj
{
	void debug_info::update(dur)
	{
		m_background.setSize(m_text.get_size());
		auto text(mlk::stl_string::str_format("FPS:      %%\nFDur:     %%\nEntities: %%",
				  m_game.get_updater().get_fps(),
				  m_game.get_updater().get_frameduration(),
				  m_game.get_world().num_entities()));
		m_text.set_text(text);
	}

	void debug_info::render()
	{
		m_game.render_object(m_background);
		m_game.render_object(m_text);
	}
}


#endif // RJ_GAME_DEBUG_INFO_INL
