//
// Copyright (c) 2013-2017 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_BACKGROUND_BASIC_BACKGROUND_COMPONENT_HPP
#define RJ_GAME_BACKGROUND_BASIC_BACKGROUND_COMPONENT_HPP


#include <rectojump/global/common.hpp>

#include <mlk/time/simple_timer.h>


namespace rj
{
	class basic_background_component
	{
	protected:
		rndr& m_render;
		vec2f m_position;
		vec2f m_size;
		mlk::tm::simple_timer m_lifetime_timer;
		float m_rotationstep;
		vec2f m_movestep;
		bool m_infinite_lifetime;
		bool m_destroyed{false};

	public:
		basic_background_component(rndr& r, const vec2f& pos, const vec2f& size, mlk::ullong lifetime, float rotatiostep, const vec2f& movestep) :
			m_render{r},
			m_position{pos},
			m_size{size},
			m_lifetime_timer{lifetime},
			m_rotationstep{rotatiostep},
			m_movestep{movestep},
			m_infinite_lifetime{lifetime == 0}
		{ }

		virtual void update(dur) = 0;
		virtual void render() = 0;
		bool is_destroyed()
		{return m_destroyed;}
	};
}



#endif // RJ_GAME_BACKGROUND_BASIC_BACKGROUND_COMPONENT_HPP
