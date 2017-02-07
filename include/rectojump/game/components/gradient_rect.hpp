//
// Copyright (c) 2013-2017 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_COMPONENTS_GRADIENT_RECT_HPP
#define RJ_GAME_COMPONENTS_GRADIENT_RECT_HPP

#include <rectojump/global/common.hpp>

#include <mlk/graphics/graphics_utl.h>

#include <SFML/Graphics.hpp>

namespace rj
{
	class gradient_rect : public sf::Drawable
	{
		vec2f m_position{0.f, 0.f};
		vec2f m_size;
		sf::VertexArray m_verts{sf::Quads};

		sf::Color m_startcolor;
		sf::Color m_endcolor;

		std::size_t m_num_gradient_points;
		float m_step_ratio;

	public:
		gradient_rect(const vec2f& size = {0.f, 0.f},
					  const sf::Color& startcolor = {255, 255, 255},
					  const sf::Color& endcolor = {255, 255, 255},
					  std::size_t gradient_points = 1)
			: m_size{size},
			  m_startcolor{startcolor},
			  m_endcolor{endcolor},
			  m_num_gradient_points{gradient_points},
			  m_step_ratio{1.f / m_num_gradient_points}
		{
			this->recalculate();
		}

		void set_size(const vec2f& size) noexcept
		{
			m_size = size;
			this->recalculate();
		}

		void set_position(const vec2f& pos) noexcept
		{
			m_position = pos;
			this->recalculate();
		}

		void set_startcolor(const sf::Color& color) noexcept
		{
			m_startcolor = color;
			this->recalculate();
		}

		void set_endcolor(const sf::Color& color) noexcept
		{
			m_endcolor = color;
			this->recalculate();
		}

		void set_gradient_points(std::size_t num) noexcept
		{
			m_num_gradient_points = num;
			this->recalculate();
		}

		void move(const vec2f& offset) noexcept
		{
			this->set_position(m_position + offset);
		}

		const vec2f& get_size() const noexcept { return m_size; }

		const vec2f& get_position() const noexcept { return m_position; }

		const sf::Color& get_startcolor() const noexcept
		{
			return m_startcolor;
		}

		const sf::Color& get_endcolor() const noexcept { return m_endcolor; }

		std::size_t num_gradient_points() const noexcept
		{
			return m_num_gradient_points;
		}

	private:
		void recalculate() noexcept
		{
			m_verts.clear();
			m_step_ratio = 1.f / m_num_gradient_points;

			auto single_size{m_size.y / m_num_gradient_points};
			auto pos_y{m_position.y};
			auto current_ratio{0.f};

			for(auto i{0}; i < m_num_gradient_points; ++i) {
				auto current_color{mlk::gcs::color_diff(
					m_startcolor, m_endcolor, current_ratio)};

				m_verts.append(
					{{m_position.x, pos_y + single_size}, current_color});
				m_verts.append({{m_position.x, pos_y}, current_color});
				m_verts.append(
					{{m_size.x + m_position.x, pos_y}, current_color});
				m_verts.append({{m_size.x + m_position.x, pos_y + single_size},
								current_color});

				pos_y += single_size;
				current_ratio += m_step_ratio;
			}
		}

		void draw(sf::RenderTarget& target,
				  sf::RenderStates states) const override
		{
			target.draw(m_verts, states);
		}
	};
}

#endif// RJ_GAME_COMPONENTS_GRADIENT_RECT_HPP
