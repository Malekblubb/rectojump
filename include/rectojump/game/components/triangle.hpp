//
// Copyright (c) 2013-2017 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_COMPONENTS_TRIANGLE_HPP
#define RJ_GAME_COMPONENTS_TRIANGLE_HPP

#include <rectojump/game/basic_entity.hpp>

#include <SFML/Graphics.hpp>

namespace rj
{
	class triangle final : public basic_entity<sf::CircleShape>
	{
	public:
		triangle(const vec2f& pos, float radius = 24.f,
				 const vec2f& velocity = {-0.4f, 0.f})
			: basic_entity{pos, velocity}
		{
			m_render_object.setPointCount(3);
			m_render_object.setRadius(radius);
			m_render_object.setOrigin(vec2f{radius, radius});
			m_render_object.setPosition({pos.x, pos.y + 10.f});
			this->set_propertie(entity_propertie::death);
		}

		float top_out() const noexcept override
		{
			return m_render_object.getGlobalBounds().top;
		}

		float bottom_out() const noexcept override
		{
			return this->top_out() + m_render_object.getRadius();
		}

		float left_out() const noexcept override
		{
			return m_render_object.getGlobalBounds().left;
		}

		float right_out() const noexcept override
		{
			return this->left_out() + m_render_object.getRadius();
		}

		void update(dur duration) override
		{
			m_render_object.move(m_velocity * duration);
		}
	};
}

#endif// RJ_GAME_COMPONENTS_TRIANGLE_HPP
