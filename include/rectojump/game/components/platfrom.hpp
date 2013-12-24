//
// Copyright (c) 2013 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_COMPONENTS_PLATFORM_HPP
#define RJ_GAME_COMPONENTS_PLATFORM_HPP


#include <rectojump/game/entity_rect.hpp>


namespace rj
{
	class platform : public entity_rect
	{
	protected:
		void update(dur duration) override
		{m_render_object.move(-0.4f * duration, 0.f);}

	public:
		platform(const sf::Vector2f& pos, const sf::Vector2f& size = {20.f, 20.f}) :
			entity_rect{pos, size}
		{ }
	};
}



#endif // RJ_GAME_COMPONENTS_PLATFORM_HPP
