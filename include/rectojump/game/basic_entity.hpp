//
// Copyright (c) 2013 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_BASIC_ENTITY_HPP
#define RJ_GAME_BASIC_ENTITY_HPP


#include "entity_base.hpp"


namespace rj
{
	template<typename T>
	class basic_entity : public entity_base
	{
	protected:
		T m_render_object;
		sf::Vector2f m_velocity{0.f, 0.f};

		virtual void update(dur duration) override = 0;
		virtual void render() override;
		virtual void init() override = 0;

	public:
		basic_entity(const sf::Vector2f& pos, const sf::Vector2f& velocity) :
			m_velocity{velocity}
		{m_render_object.setPosition(pos);}

		virtual ~basic_entity() = default;

		T& render_object() noexcept
		{return m_render_object;}

		// position, collision
		float pos_x() const noexcept override
		{return m_render_object.getPosition().x;}

		float pos_y() const noexcept override
		{return m_render_object.getPosition().y;}

		float top_out() const noexcept override
		{return this->pos_y() - (m_render_object.getSize().y / 2.f);}

		float bottom_out() const noexcept override
		{return this->pos_y() + (m_render_object.getSize().y / 2.f);}

		float left_out() const noexcept override
		{return this->pos_x() - (m_render_object.getSize().x / 2.f);}

		float right_out() const noexcept override
		{return this->pos_x() + (m_render_object.getSize().x / 2.f);}
	};
}


#endif // RJ_GAME_BASIC_ENTITY_HPP
