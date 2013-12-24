//
// Copyright (c) 2013 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_ENTITY_RECT_HPP
#define RJ_GAME_ENTITY_RECT_HPP


#include "entity.hpp"

#include <SFML/Graphics.hpp>


namespace rj
{
	class entity_rect : public basic_entity<sf::RectangleShape>
	{
	protected:
		virtual void update(dur duration) override = 0;
		virtual void init() override = 0;

	public:
		entity_rect(const sf::Vector2f& pos, const sf::Vector2f& size) :
			basic_entity{pos}
		{m_render_object.setSize(size);}
	};
}



#endif // RJ_GAME_ENTITY_RECT_HPP
