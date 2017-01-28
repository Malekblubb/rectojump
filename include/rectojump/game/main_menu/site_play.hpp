//
// Copyright (c) 2013-2017 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_MAIN_MENU_SITE_PLAY_HPP
#define RJ_GAME_MAIN_MENU_SITE_PLAY_HPP


#include <rectojump/global/config_settings.hpp>
#include <rectojump/shared/level_manager/level_manager.hpp>
#include <rectojump/ui/level_widget.hpp>
#include <rectojump/ui/stacked_widget.hpp>


namespace rj
{
	template<typename Overlay>
	class site_play
	{
		Overlay& m_overlay;
		level_manager& m_lvmgr;

		ui::stacked_widget& m_sites;

	public:
		site_play(Overlay& ov) :
			m_overlay{ov},
			m_lvmgr{ov.mainmenu().gamehandler().levelmgr()},
			m_sites{ov.sites()}
		{ }

		void construct()
		{
			auto& site_play(m_sites.get("play"));
			auto& font(m_overlay.mainmenu().gamehandler().datastore().template get<sf::Font>
					   (settings::text_font()));

			// create level_widget
			auto level_widget(m_sites.add_object<ui::level_widget>(
								  "play", m_overlay.mainmenu().gamehandler().gamewindow(),
								  m_sites.size(), m_sites.pos()));

			// add levels
			for(const auto& a : m_lvmgr.get_levels())
				level_widget->add_item(a.second, font);

			// set camera on render
			site_play.on_render = [level_widget]{level_widget->activate_cam();};

			// add input
			m_overlay.mainmenu().gamehandler().template add_input<state::main_menu>
					([this, level_widget](const vec2f&)
			{
				if(m_sites.active("play"))
					level_widget->scroll_up();
			}, wheel::up);

			m_overlay.mainmenu().gamehandler().template add_input<state::main_menu>
					([this, level_widget](const vec2f&)
			{
				if(m_sites.active("play"))
					level_widget->scroll_down();
			}, wheel::down);
		}
	};
}


#endif // RJ_GAME_MAIN_MENU_SITE_PLAY_HPP


