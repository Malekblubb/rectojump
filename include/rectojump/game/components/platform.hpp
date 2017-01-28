//
// Copyright (c) 2013-2017 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_COMPONENTS_PLATFORM_HPP
#define RJ_GAME_COMPONENTS_PLATFORM_HPP


#include <rectojump/game/entity_rect.hpp>


namespace rj
{
	class platform final : public entity_rect
	{
	public:
		platform(const vec2f& pos, const vec2f& size = {20.f, 20.f},
				 const vec2f& velocity = {-0.3f, 0.f}) :
			entity_rect{pos, size, velocity}
		{m_render_object.setOrigin(size.x / 2, size.y / 2);}

		~platform() = default;

		void update(dur duration) override
		{m_render_object.move(m_velocity * duration);}
	};
}



#endif // RJ_GAME_COMPONENTS_PLATFORM_HPP
