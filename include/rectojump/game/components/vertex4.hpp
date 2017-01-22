//
// Copyright (c) 2013-2017 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_COMPONENTS_VERTEX4_HPP
#define RJ_GAME_COMPONENTS_VERTEX4_HPP


#include <rectojump/global/common.hpp>


namespace rj
{
	class vertex4 : public sf::Drawable
	{
		sf::VertexArray m_verts{sf::Quads, 4};

	public:
		vertex4(const vec2f& size = {}, const vec2f& pos = {})
		{
			m_verts[0].position = {pos.x, pos.y + size.y};
			m_verts[1].position = {pos.x, pos.y};
			m_verts[2].position = {pos.x + size.x, pos.y};
			m_verts[3].position = {pos.x + size.x, pos.y + size.y};
		}

	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const
		{
			target.draw(m_verts, states);
		}
	};
}


#endif // RJ_GAME_COMPONENTS_VERTEX4_HPP

