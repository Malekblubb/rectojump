//
// Copyright (c) 2013 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_COMPONENTS_TRIANGLE_HPP
#define RJ_GAME_COMPONENTS_TRIANGLE_HPP


#include <rectojump/game/basic_entity.hpp>

#include <SFML/Graphics.hpp>


namespace rj
{
	class triangle : public basic_entity<sf::CircleShape>
	{

	public:
		triangle(const sf::Vector2f& pos) :
			basic_entity{pos, {-0.3f, 0.f}}
		{
			m_render_object.setPointCount(3);
			m_render_object.setRadius(15.f);
			m_render_object.setOrigin(7.5f, 7.5f);
		}

		float top_out() const noexcept override
		{return this->pos_y() - (m_render_object.getRadius() / 2.f);}

		float bottom_out() const noexcept override
		{return this->pos_y() + (m_render_object.getRadius() / 2.f);}

		float left_out() const noexcept override
		{return this->pos_x() - (m_render_object.getRadius() / 2.f);}

		float right_out() const noexcept override
		{return this->pos_x() + (m_render_object.getRadius() / 2.f);}

	private:
		void update(dur duration) override
		{m_render_object.move(m_velocity * duration);}
	};
}


#endif // RJ_GAME_COMPONENTS_TRIANGLE_HPP