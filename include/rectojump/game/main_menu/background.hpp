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
	template<typename Main_Menu>
	class background
	{
		Main_Menu& m_mainmenu;
		background_manager& m_backgroundmgr;

		sf::Texture m_sides_tx;
		const sf::Color& m_fillcolor;
		mlk::tm::simple_timer m_timer{300};

	public:
		background(Main_Menu& mm) :
			m_mainmenu{mm},
			m_backgroundmgr{mm.get_gamehandler().get_backgroundmgr()},
			m_sides_tx{mm.get_gamehandler().get_datamgr().template get_as<sf::Texture>("menu_side.png")},
			m_fillcolor{mm.get_act_fontcolor()}
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
				auto rotatestep(mlk::rnd(-0.1f, 0.5f));
				vec2f movestep{mlk::rnd(-0.1f, 0.5f), mlk::rnd(0.05f, 0.5f)};

				auto object_type(mlk::rnd(0, 1));

				if(object_type)
				{
					auto ptr(m_backgroundmgr.create_object<star5>(vec2f{pos_x, 0.f}, vec2f{0.f, length}, 5000, rotatestep, movestep));
					ptr->render_object().setFillColor({m_fillcolor.r, m_fillcolor.g, m_fillcolor.b, 100});
					m_timer.restart(mlk::rnd<mlk::ullong>(100, 300));
				}
				else
				{
//					auto ptr(m_backgroundmgr.create_object<sf::RectangleShape>(vec2f{pos_x, 0.f}, vec2f{50.f, 50.f}, 5000, rotatestep, movestep));
//					ptr->render_object().setFillColor({m_fillcolor.r, m_fillcolor.g, m_fillcolor.b, 100});
//					ptr->render_object().setOrigin(ptr->render_object().getSize() / 2.f);
//					m_timer.restart(mlk::rnd<mlk::ullong>(100, 300));

					auto ptr(m_backgroundmgr.create_object<triangles4>(vec2f{pos_x, 0.f}, vec2f{15.5f, 30.f}, 5000, rotatestep, movestep));
					ptr->render_object().setColor({m_fillcolor.r, m_fillcolor.g, m_fillcolor.b, 100});
//					ptr->render_object().setOrigin(ptr->render_object().getSize() / 2.f);
					m_timer.restart(mlk::rnd<mlk::ullong>(100, 300));
				}


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
