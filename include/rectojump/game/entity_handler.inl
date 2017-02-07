#ifndef ENTITY_HANDLER_INL
#define ENTITY_HANDLER_INL

#include "entity_handler.hpp"
#include <rectojump/core/game_handler.hpp>

namespace rj
{
	void entity_handler::on_player_death()
	{
		if(m_gamehandler != nullptr) m_gamehandler->on_player_death();
	}
}

#endif// ENTITY_HANDLER_INL
