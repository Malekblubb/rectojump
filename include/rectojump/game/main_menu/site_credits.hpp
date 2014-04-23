//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_MAIN_MENU_SITE_CREDITS_HPP
#define RJ_GAME_MAIN_MENU_SITE_CREDITS_HPP


#include <rectojump/ui/stacked_widget.hpp>


namespace rj
{
	template<typename Overlay>
	class site_credits
	{
		Overlay& m_overlay;
		ui::stacked_widget& m_sites;

	public:
		site_credits(Overlay& ov) :
			m_overlay{ov},
			m_sites{ov.sites()}
		{ }

		void construct()
		{
			auto text(m_sites.add_object<sf::Text>("credits", "", m_overlay.mainmenu().gamehandler().datastore().template get<sf::Font>(glob::text_font), glob::text_size));
			text->setString("Programmer: Christoph Malek <>\n"
							"Graphics: -\n"
							"Icons: Adam Whitcroft <http://adamwhitcroft.com/batch/>\n\n"
							"Libs: SFML, zlib");
			text->setColor(to_rgb("#555555"));
			text->setPosition({(m_sites.bounds().width - text->getGlobalBounds().width) / 2.f, 150.f});
		}
	};
}


#endif // RJ_GAME_MAIN_MENU_SITE_CREDITS_HPP








