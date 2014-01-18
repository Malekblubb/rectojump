//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_COMPONENTS_TRIANGLES4_HPP
#define RJ_GAME_COMPONENTS_TRIANGLES4_HPP


#include <rectojump/global/common.hpp>

#include <SFML/Graphics.hpp>


namespace rj
{
	class triangles4 : public sf::Drawable, private sf::Transformable
	{
		sf::VertexArray m_verts{sf::Triangles};
		vec2f m_origin{0.f, 0.f};
		vec2f m_size{50.f, 200.f};
		sf::Color m_color;

	public:
		triangles4(const vec2f& size, const sf::Color& color = {255, 255, 255}) :
			m_size{size},
			m_color{color}
		{this->init();}

		void set_origin(const vec2f& pos)
		{sf::Transformable::setOrigin(pos); m_origin = pos; this->recalculate();}

		void set_position(const vec2f& pos)
		{sf::Transformable::setPosition(pos);}

		void set_triangle_size(const vec2f& size) noexcept
		{m_size = size; this->recalculate();}

		void set_color(const sf::Color& color) noexcept
		{m_color = color; this->recolor();}

		void rotate(float angle)
		{sf::Transformable::rotate(angle);}

		const vec2f& get_origin() const noexcept
		{return sf::Transformable::getOrigin();}

		const vec2f& get_position() const noexcept
		{return sf::Transformable::getPosition();}

		const vec2f& get_triangle_size() const noexcept
		{return m_size;}

		const sf::Color& get_color() const noexcept
		{return m_color;}

	private:
		void init()
		{
			// top
			m_verts.append({m_origin, m_color});
			m_verts.append({{m_origin.x + m_size.x, m_origin.y - m_size.y}, m_color});
			m_verts.append({{m_origin.x - m_size.x, m_origin.y - m_size.y}, m_color});

			// bottom
			m_verts.append({m_origin, m_color});
			m_verts.append({{m_origin.x + m_size.x, m_origin.y + m_size.y}, m_color});
			m_verts.append({{m_origin.x - m_size.x, m_origin.y + m_size.y}, m_color});

			// right
			m_verts.append({m_origin, m_color});
			m_verts.append({{m_origin.x + m_size.y, m_origin.y - m_size.x}, m_color});
			m_verts.append({{m_origin.x + m_size.y, m_origin.y + m_size.x}, m_color});

			// left
			m_verts.append({m_origin, m_color});
			m_verts.append({{m_origin.x - m_size.y, m_origin.y - m_size.x}, m_color});
			m_verts.append({{m_origin.x - m_size.y, m_origin.y + m_size.x}, m_color});
		}

		void recalculate() noexcept
		{
			// top
			m_verts[0].position = m_origin;
			m_verts[1].position = {m_origin.x + m_size.x, m_origin.y - m_size.y};
			m_verts[2].position = {m_origin.x - m_size.x, m_origin.y - m_size.y};

			// bottom
			m_verts[3].position = m_origin;
			m_verts[4].position = {m_origin.x + m_size.x, m_origin.y + m_size.y};
			m_verts[5].position = {m_origin.x - m_size.x, m_origin.y + m_size.y};

			// right
			m_verts[6].position = m_origin;
			m_verts[7].position = {m_origin.x + m_size.y, m_origin.y - m_size.x};
			m_verts[8].position = {m_origin.x + m_size.y, m_origin.y + m_size.x};

			// left
			m_verts[9].position = m_origin;
			m_verts[10].position = {m_origin.x - m_size.y, m_origin.y - m_size.x};
			m_verts[11].position = {m_origin.x - m_size.y, m_origin.y + m_size.x};
		}

		void recolor()
		{
			auto begin(&m_verts[0]);
			auto end(&m_verts[m_verts.getVertexCount()]);

			for(auto iter(begin); iter != end; ++iter)
				iter->color = m_color;
		}

		void draw(sf::RenderTarget& target, sf::RenderStates states) const
		{
			auto tmp(states);
			tmp.transform = this->getTransform();
			target.draw(m_verts, tmp);
		}
	};
}


#endif // RJ_GAME_COMPONENTS_TRIANGLES4_HPP
