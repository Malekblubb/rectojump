//
// Copyright (c) 2013-2020 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_COMPONENTS_ENT_TRIANGLES4_HPP
#define RJ_GAME_COMPONENTS_ENT_TRIANGLES4_HPP

#include <rectojump/game/entity_rect.hpp>

#include <SFML/Graphics.hpp>

namespace rj
{
	class ent_triangles4 final : public entity_rect
	{
		static constexpr float m_rotate{.3f};

	public:
		ent_triangles4(const vec2f& pos, const vec2f& size = {30.f, 30.f},
					   const vec2f& velocity = {-0.4f, 0.f})
			: entity_rect{pos, size, velocity}
		{
			m_render_object.setOrigin(size.x / 2, size.y / 2);
			this->set_propertie(entity_propertie::death);
		}

		~ent_triangles4() = default;

		void update(dur duration) override
		{
			m_render_object.move(m_velocity * duration);
			m_render_object.rotate(m_rotate);
		}
	};
}// namespace rj

#endif// RJ_GAME_COMPONENTS_ENT_TRIANGLES4_HPP
