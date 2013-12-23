//
// Copyright (c) 2013 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_BASIC_ENTITY_HPP
#define RJ_GAME_BASIC_ENTITY_HPP


#include "entity_base.hpp"


namespace rj
{
	class game;

	template<typename T>
	class basic_entity : public entity_base
	{
	protected:
		T m_render_object;

		virtual void update(dur duration) override = 0;
		virtual void render() override;

	public:
		basic_entity(game& g, const sf::Vector2f& pos) :
			entity_base{g}
		{m_render_object.setPosition(pos);}

		T& render_object() noexcept
		{return m_render_object;}
	};
}


#endif // RJ_GAME_BASIC_ENTITY_HPP
