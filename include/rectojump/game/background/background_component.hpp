//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_BACKGROUND_BACKGROUND_COMPONENT_HPP
#define RJ_GAME_BACKGROUND_BACKGROUND_COMPONENT_HPP


#include "basic_background_component.hpp"
#include <rectojump/core/render.hpp>
#include <rectojump/game/components/star5.hpp>
#include <rectojump/global/common.hpp>

#include <mlk/time/simple_timer.h>


namespace rj
{
	class game;

	template<typename Drawable_Type>
	class background_component : public basic_background_component
	{
		Drawable_Type m_render_object;

	public:
		background_component(game& g, const vec2f& pos, const vec2f& size, mlk::ullong lifetime, float rotatiostep, const vec2f& movestep) :
			basic_background_component{g, pos, size, lifetime, rotatiostep, movestep}
		{
			this->set_properties();
			m_lifetime_timer.run();
		}

		void set_properties() noexcept
		{
			m_render_object.setPosition(m_position);
			m_render_object.setSize(m_size);
		}

		void update(dur duration) override
		{
			if(!m_infinite_lifetime)
				if(m_lifetime_timer.timed_out())
				{
					m_destroyed = true;
					return;
				}
			m_render_object.move(m_movestep * duration);
			m_render_object.rotate(m_rotationstep * duration);
		}

		void render() override
		{
			if(m_destroyed)
				return;
			rndr::ro(m_game, m_render_object);
		}

		Drawable_Type& render_object() noexcept
		{return m_render_object;}
	};

	template<>
	inline void background_component<star5>::set_properties() noexcept
	{
		m_render_object.setPosition(m_position);
		m_render_object.set_length(m_size.y);
	}
}



#endif // RJ_GAME_BACKGROUND_BACKGROUND_COMPONENT_HPP
