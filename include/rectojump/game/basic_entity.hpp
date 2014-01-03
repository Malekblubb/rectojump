//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_BASIC_ENTITY_HPP
#define RJ_GAME_BASIC_ENTITY_HPP


#include "entity_base.hpp"
#include <rectojump/core/render.hpp>


namespace rj
{

	template<typename T>
	class basic_entity : public entity_base
	{
	protected:
		T m_render_object;
		vec2f m_velocity{0.f, 0.f};

		virtual void update(dur duration) override = 0;
		virtual void render() override
		{render::render_object(*m_game, m_render_object);}
		virtual void init() override { }

	public:
		basic_entity(const vec2f& pos, const vec2f& velocity) :
			m_velocity{velocity}
		{m_render_object.setPosition(pos);}

		virtual ~basic_entity() = default;

		T& render_object() noexcept
		{return m_render_object;}

		// position, collision
		const vec2f& pos() const noexcept override
		{return m_render_object.getPosition();}

		float pos_x() const noexcept override
		{return this->pos().x;}

		float pos_y() const noexcept override
		{return this->pos().y;}

		virtual float top_out() const noexcept override = 0;
		virtual float bottom_out() const noexcept override = 0;
		virtual float left_out() const noexcept override = 0;
		virtual float right_out() const noexcept override = 0;
	};
}


#endif // RJ_GAME_BASIC_ENTITY_HPP
