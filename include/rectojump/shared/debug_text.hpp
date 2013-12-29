//
// Copyright (c) 2013 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_SHARED_DEBUG_TEXT_HPP
#define RJ_SHARED_DEBUG_TEXT_HPP


#include <rectojump/global/errors.hpp>

#include <mlk/log/log.h>

#include <SFML/Graphics.hpp>


namespace rj
{
	class debug_text : public sf::Drawable
	{
		std::string m_current_str;
		sf::Texture m_texture;
		sf::VertexArray m_verts{sf::Quads};
		bool m_valid{false};

	public:
		debug_text(const std::string& tile_map_path) :
			m_valid{m_texture.loadFromFile(tile_map_path)}
		{
			if(!m_valid)
				mlk::lerr(errors::io_open_file) << "filename=" << tile_map_path;
		}

		void set_text(const std::string& text)
		{m_current_str = text; this->refresh();}

	private:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates) const override
		{target.draw(m_verts, &m_texture);}

		void reset() noexcept
		{m_verts.clear();}

		void refresh() noexcept
		{
			if(!m_valid)
				return;

			this->reset();

			auto posx(0.f), posy(0.f);
			for(const auto& a : m_current_str)
			{
				if(a == '\n')
				{
					posy += 16.f;
					posx = 0.f;
					continue;
				}

				// tile position
				auto tiley(static_cast<float>((a / 16) * 16)), tilex(static_cast<float>((a % 16) * 16));

				// create quad
				m_verts.append({{posx, posy + 16.f}, {tilex, tiley + 16.f}});
				m_verts.append({{posx, posy}, {tilex, tiley}});
				m_verts.append({{posx + 16.f, posy}, {tilex + 16.f, tiley}});
				m_verts.append({{posx + 16.f, posy + 16.f}, {tilex + 16.f, tiley + 16.f}});

				posx += 16.f;
			}
		}
	};
}

#endif // RJ_SHARED_DEBUG_TEXT_HPP
