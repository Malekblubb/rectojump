//
// Copyright (c) 2013-2017 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_ENTITY_HANDLER_INL
#define RJ_GAME_ENTITY_HANDLER_INL

#include "entity_handler.hpp"
#include <rectojump/core/game_handler.hpp>

namespace rj
{
	void entity_handler::on_player_death()
	{
		if(m_gamehandler != nullptr) m_gamehandler->on_player_death();
	}
}

#endif// RJ_GAME_ENTITY_HANDLER_INL
