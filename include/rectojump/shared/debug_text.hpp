//
// Copyright (c) 2013 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_SHARED_DEBUG_TEXT_HPP
#define RJ_SHARED_DEBUG_TEXT_HPP


#include <rectojump/global/common.hpp>
#include <rectojump/global/errors.hpp>

#include <mlk/log/log.h>

#include <SFML/Graphics.hpp>


namespace rj
{
	class debug_text : public sf::Drawable
	{
		std::string m_current_str;
		sf::Color m_color;
		sf::Texture m_texture;
		sf::VertexArray m_verts{sf::Quads};
		vec2f m_size{0.f, 0.f};
		bool m_valid{false};

	public:
		debug_text(const std::string& tile_map_path, const sf::Color& color = {255, 255, 255}) :
			m_valid{m_texture.loadFromFile(tile_map_path)},
			m_color{color}
		{
			if(!m_valid)
				mlk::lerr(errors::io_open_file) << "filename=" << tile_map_path;
		}

		void set_text(const std::string& text) noexcept
		{m_current_str = text; this->refresh();}

		void set_color(const sf::Color& color) noexcept
		{m_color = color;}

		const vec2f& get_size() const noexcept
		{return m_size;}

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
			auto lines(0);
			m_size.x = posx; m_size.y = posy;
			for(const auto& a : m_current_str)
			{
				if(a == '\n')
				{
					posy += 16.f;
					posx = 0.f;
					++lines;
					continue;
				}

				// tile position
				auto tiley(static_cast<float>((a / 16) * 16)), tilex(static_cast<float>((a % 16) * 16));

				// create quad
				m_verts.append({{posx, posy + 16.f}, m_color, {tilex, tiley + 16.f}});
				m_verts.append({{posx, posy}, m_color, {tilex, tiley}});
				m_verts.append({{posx + 16.f, posy}, m_color, {tilex + 16.f, tiley}});
				m_verts.append({{posx + 16.f, posy + 16.f}, m_color, {tilex + 16.f, tiley + 16.f}});

				posx += 16.f;
				if(posx > m_size.x) m_size.x = posx;
			}
			m_size.y = (lines + 1) * 16.f;
		}
	};
}

#endif // RJ_SHARED_DEBUG_TEXT_HPP
