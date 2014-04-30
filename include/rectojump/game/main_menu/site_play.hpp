//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_MAIN_MENU_SITE_PLAY_HPP
#define RJ_GAME_MAIN_MENU_SITE_PLAY_HPP


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
			auto& font(m_overlay.mainmenu().gamehandler().datastore().template get<sf::Font>(glob::text_font));

			auto level_widget(m_sites.add_object<ui::level_widget>("play", m_overlay.mainmenu().gamehandler().gamewindow(), m_sites.size(), m_sites.pos()));
			level_widget->add_item(m_lvmgr.get_level("test.rjl"), font);

			site_play.on_render = [level_widget]{level_widget->activate_cam();};
		}
	};
}


#endif // RJ_GAME_MAIN_MENU_SITE_PLAY_HPP


