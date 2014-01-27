//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_EDITOR_EDITOR_ENTITY_HPP
#define RJ_GAME_EDITOR_EDITOR_ENTITY_HPP


#include <rectojump/game/entity_rect.hpp>
#include <rectojump/global/config_settings.hpp>


namespace rj
{
	class editor_entity final : public entity_rect
	{
	public:
		editor_entity(const vec2f& pos) :
			entity_rect{pos, m_size, {0.f, 0.f}}
		{
			m_render_object.setOrigin(m_size / 2.f);
			m_render_object.setOutlineThickness(2.f);
			m_render_object.setOutlineColor(settings::get_color_dark());
			m_render_object.setFillColor(settings::get_color_dark());
		}

		void update(dur)
		{ }

		void set_texture(const sf::Texture* tx)
		{m_render_object.setTexture(tx);}
	};
}


#endif // RJ_GAME_EDITOR_EDITOR_ENTITY_HPP
