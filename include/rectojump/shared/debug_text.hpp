//
// Copyright (c) 2013-2017 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_SHARED_DEBUG_TEXT_HPP
#define RJ_SHARED_DEBUG_TEXT_HPP

#include <rectojump/global/common.hpp>
#include <rectojump/global/errors.hpp>
#include <rectojump/shared/data_manager.hpp>

#include <mlk/log/log.h>

#include <SFML/Graphics.hpp>

namespace rj
{
	class debug_text : public sf::Drawable
	{
		data_manager& m_datamanager;

		std::string m_current_str;
		sf::Texture m_texture;
		sf::Color m_color;
		sf::VertexArray m_verts{sf::Quads};

		vec2f m_size{0.f, 0.f};
		float m_fontsize;
		bool m_valid{true};

	public:
		debug_text(data_manager& dm, const sf::Color& color = {255, 255, 255},
				   float fontsize = 16.f)
			: m_datamanager{dm},
			  m_texture{m_datamanager.get_as<sf::Texture>("debug_font.png")},
			  m_valid{m_datamanager.exists_id("debug_font.png")},
			  m_color{color},
			  m_fontsize{fontsize}
		{
		}

		void set_text(const std::string& text) noexcept
		{
			m_current_str = text;
			this->refresh();
		}

		void set_color(const sf::Color& color) noexcept { m_color = color; }

		void set_fontsize(float fontsize) noexcept { m_fontsize = fontsize; }

		float get_fontsize() const noexcept { return m_fontsize; }

		const vec2f& get_size() const noexcept { return m_size; }

	private:
		void draw(sf::RenderTarget& target, sf::RenderStates) const override
		{
			target.draw(m_verts, &m_texture);
		}

		void reset() noexcept { m_verts.clear(); }

		void refresh() noexcept
		{
			if(!m_valid) return;

			this->reset();

			auto posx{0.f}, posy{0.f};
			auto lines{0};
			m_size.x = posx;
			m_size.y = posy;
			for(const auto& a : m_current_str) {
				if(a == '\n') {
					posy += m_fontsize;
					posx = 0.f;
					++lines;
					continue;
				}

				// tile position
				auto tiley{static_cast<float>((a / 16) * 16)};
				auto tilex{static_cast<float>((a % 16) * 16)};

				// create quad
				m_verts.append({{posx, posy + m_fontsize},
								m_color,
								{tilex, tiley + 16.f}});
				m_verts.append({{posx, posy}, m_color, {tilex, tiley}});
				m_verts.append({{posx + m_fontsize, posy},
								m_color,
								{tilex + 16.f, tiley}});
				m_verts.append({{posx + m_fontsize, posy + m_fontsize},
								m_color,
								{tilex + 16.f, tiley + 16.f}});

				posx += m_fontsize;
				if(posx > m_size.x) m_size.x = posx;
			}
			m_size.y = (lines + 1) * m_fontsize;
		}
	};
}

#endif// RJ_SHARED_DEBUG_TEXT_HPP
