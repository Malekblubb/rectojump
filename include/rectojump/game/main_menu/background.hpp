//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_MAIN_MENU_BACKGROUND_HPP
#define RJ_GAME_MAIN_MENU_BACKGROUND_HPP


#include <rectojump/core/game_window.hpp>
#include <rectojump/core/render.hpp>
#include <rectojump/game/background/background_manager.hpp>
#include <rectojump/game/components/star5.hpp>
#include <rectojump/game/components/triangles4.hpp>
#include <rectojump/global/config_settings.hpp>
#include <rectojump/shared/data_manager.hpp>
#include <rectojump/shared/utils.hpp>

#include <mlk/time/simple_timer.h>
#include <mlk/tools/random_utl.h>


namespace rj
{
	class game;

	class background
	{
		game& m_game;
		background_manager& m_backgroundmgr;

		sf::Texture m_sides_tx;
		mlk::tm::simple_timer m_timer{300};

	public:
		background(game& g, data_manager& dm, background_manager& bgm) :
			m_game{g},
			m_backgroundmgr{bgm},
			m_sides_tx{dm.get_as<sf::Texture>("menu_side.png")}
		{this->init();}


		void update(dur duration)
		{
			this->update_bg_objs();
		}

		void update_bg_objs() noexcept
		{
			if(m_timer.timed_out())
			{
				auto size(settings::get_window_size<vec2f>());
				auto pos_x(mlk::rnd(0.f, size.x));
				auto length(mlk::rnd(30.f, 60.f));
				auto rotatestep(mlk::rnd(0.05f, 0.5f));
				vec2f movestep{mlk::rnd(0.05f, 0.5f), mlk::rnd(0.05f, 0.5f)};

				auto ptr(m_backgroundmgr.create_object<star5>(vec2f{pos_x, 0}, vec2f{0.f, length}, 3000, rotatestep, movestep));
				ptr->render_object().setFillColor({});
				m_timer.restart(mlk::rnd<mlk::ullong>(100, 500));
			}
		}

		void render()
		{ }

	private:
		void init()
		{
			auto size(settings::get_window_size<vec2f>());

			// nova
			auto triangles(m_backgroundmgr.create_object<triangles4>(vec2f{size.x / 2.f, size.y}, vec2f{200.f, 900.f}, 0, 0.1f, vec2f{0.f, 0.f}));
			triangles->render_object().setColor(to_rgb("#bdbdbd", 100));

			// background
			sf::RectangleShape tx_shape{size};
			tx_shape.setTexture(&m_sides_tx);
			m_backgroundmgr.set_tx_shape(tx_shape);
			m_backgroundmgr.set_bg_shape({size, to_rgb("#e3e3e3"), to_rgb("#e3e3e3"), 1});

			// timer
			m_timer.run();
		}
	};
}


#endif // RJ_GAME_MAIN_MENU_BACKGROUND_HPP
