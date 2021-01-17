//
// Copyright (c) 2013-2021 Christoph Malek
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
		virtual void init() override {}

	public:
		entity_rect(const vec2f& pos, const vec2f& size, const vec2f& velocity)
			: basic_entity{pos, velocity}
		{
			m_render_object.setSize(size);
			m_render_object.setOrigin(size.x / 2.f, size.y / 2.f);
		}

		virtual ~entity_rect() = default;

		float top_out() const noexcept override
		{
			return this->pos_y() - (m_render_object.getSize().y / 2.f);
		}

		float bottom_out() const noexcept override
		{
			return this->pos_y() + (m_render_object.getSize().y / 2.f);
		}

		float left_out() const noexcept override
		{
			return this->pos_x() - (m_render_object.getSize().x / 2.f);
		}

		float right_out() const noexcept override
		{
			return this->pos_x() + (m_render_object.getSize().x / 2.f);
		}
	};
}// namespace rj

#endif// RJ_GAME_ENTITY_RECT_HPP
