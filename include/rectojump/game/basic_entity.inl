//
// Copyright (c) 2013 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_BASIC_ENTITY_INL
#define RJ_GAME_BASIC_ENTITY_INL


#include "basic_entity.hpp"
#include <rectojump/core/game.hpp>


namespace rj
{
	template<typename T>
	void basic_entity<T>::render()
	{m_game->render_object(m_render_object);}
}


#endif // RJ_GAME_BASIC_ENTITY_INL
