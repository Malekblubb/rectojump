//
// Copyright (c) 2013 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_PLAYER_HPP
#define RJ_GAME_PLAYER_HPP


#include "entity_rect.hpp"


namespace rj
{
	class player : public entity_rect
	{
	protected:
		void update(dur duration) override
		{ }

	public:
		player(const sf::Vector2f& pos, const sf::Vector2f& size) :
			entity_rect{pos, size}
		{ }
	};
}


#endif // RJ_GAME_PLAYER_HPP
