//
// Copyright (c) 2013-2014 Christoph Malek
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

	public:
		camera(game_window& gw, const sf::View& v = {}) :
			m_gamewindow{gw},
			m_renderwindow{m_gamewindow.get_renderwindow()},
			m_userview{v}
		{ }

		void update(dur duration)
		{ }

		void move(const vec2f& offset) noexcept
		{m_userview.move(offset);}

		void zoom(float factor) noexcept
		{m_userview.zoom(factor);}

		void set_changes() noexcept
		{
			m_gamewindow.set_view(m_userview);
		}

		void set_view(const sf::View& v) noexcept
		{m_userview = v;}

		const vec2f& get_center() noexcept
		{return m_userview.getCenter();}

		const sf::View& get_view() const noexcept
		{return m_userview;}

		vec2f get_mapped_mousepos() const noexcept
		{return get_mousepos(m_renderwindow, m_userview);}
	};
}



#endif // RJ_GAME_CAMERA_HPP
