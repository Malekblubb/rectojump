//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_EDITOR_MOUSE_HPP
#define RJ_GAME_EDITOR_MOUSE_HPP


#include "editor_entity.hpp"
#include <rectojump/shared/input.hpp>
#include <rectojump/global/config_settings.hpp>


namespace rj
{
	class editor_mouse
	{
		rndr& m_render;

		editor_entity m_mouse_shape{{0.f, 0.f}};
		bool m_active{true};

	public:
		editor_mouse(rndr& r) :
			m_render{r}
		{

		}

		void update(dur duration)
		{
			auto new_pos(get_mousepos());
			m_mouse_shape.render_object().setPosition(new_pos);
		}

		void render()
		{
			if(m_active)
				m_render(m_mouse_shape.render_object());
		}

		void clear() noexcept
		{m_mouse_shape.render_object().setTexture(nullptr); m_active = false;}

		void set_texture(const sf::Texture* tx) noexcept
		{m_mouse_shape.render_object().setTexture(tx); m_active = true;}

		const sf::Texture* get_texture() noexcept
		{return m_mouse_shape.render_object().getTexture();}
	};
}


#endif // RJ_GAME_EDITOR_MOUSE_HPP
