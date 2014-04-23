//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_MAIN_MENU_BACKGROUND_MAIN_MENU_HPP
#define RJ_GAME_MAIN_MENU_BACKGROUND_MAIN_MENU_HPP


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
	class background_main_menu
	{
		Main_Menu& m_mainmenu;
		background_manager& m_backgroundmgr;

		const sf::Color m_fillcolor{settings::get_color_light()};
		mlk::tm::simple_timer m_timer{300};

	public:
		background_main_menu(Main_Menu& mm) :
			m_mainmenu{mm},
			m_backgroundmgr{mm.gamehandler().backgroundmgr()}
		{this->init();}


		void update(dur duration)
		{
			this->update_bg_objs(duration);
		}

		void update_bg_objs(dur duration) noexcept
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
				}
				m_timer.restart(static_cast<mlk::ullong>(mlk::rnd<mlk::ullong>(100, 300) / duration));
			}
		}

		void render()
		{ }

	private:
		void init()
		{
			auto window_size(settings::get_window_size<vec2f>());

			// nova
			auto nova(m_backgroundmgr.create_object<triangles4>(vec2f{window_size.x / 2.f, window_size.y}, vec2f{window_size.y / 3.f, window_size.x}, 0, 0.1f, vec2f{0.f, 0.f}));
			nova->render_object().setColor(to_rgb("#bdbdbd", 100));

			// timer
			m_timer.run();
		}
	};
}


#endif // RJ_GAME_MAIN_MENU_BACKGROUND_MAIN_MENU_HPP
