//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_MAIN_MENU_SITE_HOME_HPP
#define RJ_GAME_MAIN_MENU_SITE_HOME_HPP


#include <rectojump/ui/stacked_widget.hpp>


namespace rj
{
	template<typename Overlay>
	class site_home
	{
		Overlay& m_overlay;
		ui::stacked_widget& m_sites;

	public:
		site_home(Overlay& ov) :
			m_overlay{ov},
			m_sites{ov.sites()}
		{ }

		void construct()
		{
			auto logo_shape(m_sites.add_object<sf::RectangleShape>("home", vec2f{128, 128}));
			logo_shape->setTexture(&m_overlay.mainmenu().gamehandler().datastore().template get<sf::Texture>("rj_logo.png"));
			logo_shape->setPosition({(m_sites.bounds().width - logo_shape->getSize().x) / 2.f, 50.f});
		}
	};
}


#endif // RJ_GAME_MAIN_MENU_SITE_HOME_HPP

