//
// Copyright (c) 2013-2017 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_CAMERA_HPP
#define RJ_GAME_CAMERA_HPP

#include <rectojump/core/game_window.hpp>

namespace rj
{
	class camera
	{
		game_window& m_gamewindow;
		sf::RenderWindow& m_renderwindow;

		const sf::View& m_default_window_view{m_renderwindow.getDefaultView()};
		sf::View m_userview;
		vec2f m_startcenter;
		float m_reset_zoomfactor{1.f};

	public:
		camera(game_window& gw, const sf::View& v = {},
			   const sf::FloatRect& viewport = {0.f, 0.f, 1.f, 1.f})
			: m_gamewindow{gw},
			  m_renderwindow{m_gamewindow.get_renderwindow()},
			  m_userview{v},
			  m_startcenter{m_userview.getCenter()}
		{
			m_userview.setViewport(viewport);
		}

		void update(dur duration) {}

		void move(const vec2f& offset) noexcept { m_userview.move(offset); }

		void zoom(float factor) noexcept
		{
			m_userview.zoom(factor);
			m_reset_zoomfactor *= factor;
		}

		void reset_center() noexcept { m_userview.setCenter(m_startcenter); }

		void reset_zoom() noexcept
		{
			m_userview.zoom(1.f / m_reset_zoomfactor);
			m_reset_zoomfactor = 1.f;
		}

		void activate() noexcept { m_gamewindow.set_view(m_userview); }

		void set_center(const vec2f& center) noexcept
		{
			m_userview.setCenter(center);
		}

		void set_view(const sf::View& v) noexcept
		{
			m_userview = v;
			m_startcenter = m_userview.getCenter();
		}

		bool is_centered() const noexcept
		{
			return m_userview.getCenter() == m_startcenter;
		}

		bool has_moved_left() const noexcept
		{
			return m_userview.getCenter().x > m_startcenter.x;
		}

		bool has_moved_right() const noexcept
		{
			return m_userview.getCenter().x < m_startcenter.x;
		}

		bool has_moved_up() const noexcept
		{
			return m_userview.getCenter().y < m_startcenter.y;
		}

		bool has_moved_down() const noexcept
		{
			return m_userview.getCenter().y > m_startcenter.y;
		}

		const vec2f& get_center() noexcept { return m_userview.getCenter(); }

		const vec2f& get_startcenter() const noexcept { return m_startcenter; }

		const sf::View& get_view() const noexcept { return m_userview; }

		vec2f get_mapped_mousepos() const noexcept
		{
			return inp::get_mousepos(m_renderwindow, m_userview);
		}

		float get_zoomfactor() const noexcept { return m_reset_zoomfactor; }
	};
}

#endif// RJ_GAME_CAMERA_HPP
