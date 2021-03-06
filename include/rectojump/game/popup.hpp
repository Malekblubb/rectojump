//
// Copyright (c) 2013-2021 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_POPUP_HPP
#define RJ_GAME_POPUP_HPP

#include <rectojump/core/render.hpp>
#include <rectojump/global/common.hpp>
#include <rectojump/global/errors.hpp>
#include <rectojump/shared/utils.hpp>

#include <mlk/log/log.h>
#include <mlk/time/simple_timer.h>

namespace rj
{
	class popup
	{
		rndr* m_render;// need pointer here

		sf::RectangleShape m_shape;
		sf::Text m_text;
		mlk::tm::simple_timer m_lifetime_timer;
		std::uint8_t m_current_alpha{255};
		bool m_destroyed{false};

	public:
		popup(rndr* r, const sf::Font& font, const std::string& text,
			  const vec2f& pos, mlk::ulong lifetime = 3000,
			  const sf::Color& bg_color = to_rgb("#aaaaaa", 200),
			  const sf::Color& outline_color = to_rgb("#707070"),
			  const sf::Color& fontcolor = to_rgb("#505050"))
			: m_render{r}, m_text{text, font, 18}, m_lifetime_timer{lifetime}
		{
			auto size{m_text.getGlobalBounds()};
			vec2f origin{size.width / 2.f, size.height / 2.f};
			m_text.setOrigin(origin);
			m_text.setPosition(pos);
			m_text.setFillColor(fontcolor);
			m_shape.setOrigin(origin);
			m_shape.setPosition(pos);
			m_shape.setSize({size.width, size.height + 10.f});
			m_shape.setOutlineThickness(1.f);
			m_shape.setOutlineColor(outline_color);
			m_shape.setFillColor(bg_color);
			m_lifetime_timer.run();
		}

		void update(dur)
		{
			if(m_lifetime_timer.timed_out()) {
				if(m_current_alpha <= 0) {
					m_destroyed = true;
					return;
				}

				--m_current_alpha;
				auto scolor{m_shape.getFillColor()};
				scolor.a = m_current_alpha;
				m_shape.setFillColor(scolor);

				auto ocolor{m_shape.getOutlineColor()};
				ocolor.a = m_current_alpha;
				m_shape.setOutlineColor(ocolor);

				auto tcolor{m_text.getFillColor()};
				tcolor.a = m_current_alpha;
				m_text.setFillColor(tcolor);
			}
		}

		void render()
		{
			if(m_render == nullptr) {
				mlk::lerr(errors::cl_nullptr_access)["rj::popup"];
				return;
			}
			(*m_render)(m_shape, m_text);
		}

		bool is_destroyed() const noexcept { return m_destroyed; }
	};
}

#endif// RJ_GAME_POPUP_HPP
