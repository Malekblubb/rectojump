//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_MAIN_MENU_SITE_HOME_HPP
#define RJ_GAME_MAIN_MENU_SITE_HOME_HPP


#include <rectojump/ui/sf_widget.hpp>
#include <rectojump/ui/stacked_widget.hpp>


namespace rj
{
	template<typename Overlay>
	class site_home
	{
		Overlay& m_overlay;
		data_store_type& m_datasore;

		ui::stacked_widget& m_sites;

	public:
		site_home(Overlay& ov) :
			m_overlay{ov},
			m_datasore{m_overlay.mainmenu().gamehandler().datastore()},
			m_sites{ov.sites()}
		{ }

		void construct()
		{
			auto logo_shape(m_sites.add_object<widget::rectangle_shape>("home", vec2f{128, 128}));
			logo_shape->get().setTexture(&m_datasore.template get<sf::Texture>("rj_logo.png"));
			logo_shape->get().setPosition({(m_sites.bounds().width - logo_shape->get().getSize().x) / 2.f, 50.f});

			auto username(m_sites.add_object<ui::textbox>("home", vec2f{150.f, 50.f}, vec2f{20, 20}, m_datasore.template get<sf::Font>(glob::text_font), "text"));
			username->setOutlineThickness(2.f);
			username->setOutlineColor(sf::Color::Red);
			username->setTextColor(sf::Color::Green);
			username->setCursorColor(sf::Color::Green);
		}
	};
}


#endif // RJ_GAME_MAIN_MENU_SITE_HOME_HPP

