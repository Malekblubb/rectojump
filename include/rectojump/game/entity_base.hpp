//
// Copyright (c) 2013 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_ENTITY_BASE_HPP
#define RJ_GAME_ENTITY_BASE_HPP


#include "entity_groups.hpp"
#include <rectojump/global/common.hpp>


namespace rj
{
	class entity_handler;
	class game;

	class entity_base
	{
		friend class entity_handler;

		int m_id{-1};
		bool m_is_registered{false};


	protected:
		game& m_game;

		virtual void update(dur duration) = 0;
		virtual void render() = 0;

	public:
		entity_base(game& g) :
			m_game{g}
		{ }
	};
}


#endif // RJ_GAME_ENTITY_BASE_HPP
