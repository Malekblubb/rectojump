//
// Copyright (c) 2013-2017 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_BASIC_ENTITY_HPP
#define RJ_GAME_BASIC_ENTITY_HPP

#include "entity_base.hpp"
#include <rectojump/core/render.hpp>
#include <rectojump/global/errors.hpp>

#include <mlk/log/log.h>

namespace rj
{
	template <typename T>
	class basic_entity : public entity_base
	{
	protected:
		T m_render_object;
		vec2f m_velocity{0.f, 0.f};
		const vec2f m_size{48.f, 48.f};

		virtual void update(dur duration) override = 0;
		virtual void init() override {}

	public:
		using value_type = T;

		basic_entity(const vec2f& pos, const vec2f& velocity)
			: m_velocity{velocity}
		{
			m_render_object.setPosition(pos);
		}

		virtual ~basic_entity() override = default;

		virtual void render() override
		{
			if(m_render == nullptr)
			{
				mlk::lerr(errors::cl_nullptr_access)["rj::basic_entity<T>"];
				return;
			}
			(*m_render)(m_render_object);
		}

		void activate_outlines(bool on)
		{
			if(on)
			{
				m_render_object.setOutlineThickness(1.f);
				m_render_object.setOutlineColor(sf::Color::Red);
			}
			else
				m_render_object.setOutlineThickness(0.f);
		}

		T& render_object() noexcept { return m_render_object; }

		const T& get_render_object() const noexcept { return m_render_object; }

		// position, collision
		const vec2f size() const noexcept override
		{
			return m_render_object.getSize();
		}

		const vec2f pos() const noexcept override
		{
			return m_render_object.getPosition();
		}

		float pos_x() const noexcept override { return this->pos().x; }

		float pos_y() const noexcept override { return this->pos().y; }

		virtual float top_out() const noexcept override = 0;
		virtual float bottom_out() const noexcept override = 0;
		virtual float left_out() const noexcept override = 0;
		virtual float right_out() const noexcept override = 0;
		sf::FloatRect getGlobalBounds() const override
		{
			return m_render_object.getGlobalBounds();
		}
	};

	template <>
	inline const vec2f basic_entity<sf::CircleShape>::size() const noexcept
	{
		return {m_render_object.getRadius(), m_render_object.getRadius()};
	}
}// namespace rj

#endif// RJ_GAME_BASIC_ENTITY_HPP
