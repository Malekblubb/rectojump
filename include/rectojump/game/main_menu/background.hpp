//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_BACKGROUND_HPP
#define RJ_GAME_BACKGROUND_HPP


#include <rectojump/core/game_window.hpp>
#include <rectojump/core/render.hpp>
#include <rectojump/game/components/triangles4.hpp>
#include <rectojump/global/config_settings.hpp>
#include <rectojump/shared/utils.hpp>


namespace rj
{
	class game;

	class background
	{
		game& m_game;
		triangles4 m_nova{{200.f, 900.f}, to_rgb("#bdbdbd", 100)};

	public:
		background(game& g) :
			m_game{g}
		{this->init();}


		void update(dur duration)
		{
			m_nova.rotate(0.1f);
		}

		void render()
		{
			rndr::rmo(m_game, m_nova);
		}

	private:
		void init()
		{
			auto size(settings::get_window_size<vec2f>());
			m_nova.set_position({size.x / 2.f, size.y});
		}
	};
}


#endif // RJ_GAME_BACKGROUND_HPP
