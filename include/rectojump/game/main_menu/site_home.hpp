//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_MAIN_MENU_SITE_HOME_HPP
#define RJ_GAME_MAIN_MENU_SITE_HOME_HPP


#include <rectojump/game/popup_manager.hpp>
#include <rectojump/global/config_settings.hpp>
#include <rectojump/ui/button.hpp>
#include <rectojump/ui/sf_widget.hpp>
#include <rectojump/ui/stacked_widget.hpp>
#include <rectojump/ui/textbox.hpp>


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
			const auto& font(m_datasore.get<sf::Font>(settings::text_font()));

			// logo
			auto logo_shape(m_sites.add_object<widget::rectangle_shape>("home", vec2f{128.f, 128.f}));
			logo_shape->get().setTexture(&m_datasore.template get<sf::Texture>("rj_logo.png"));
			logo_shape->get().setPosition((m_sites.bounds().width - logo_shape->get().getSize().x) / 2.f, m_sites.size().y * 0.35f - logo_shape->get().getSize().y);

			// info text
			auto text(m_sites.add_object<widget::text>("home", "", font, settings::text_size()));
			text->get().setString("version " + settings::version());
			text->get().setColor(to_rgb("#373737"));
			text->get().setPosition((m_sites.bounds().width - text->get().getGlobalBounds().width) / 2.f, logo_shape->get().getPosition().y + 120.f);

			// textboxes
			const vec2f tbsize{250.f, 30.f};
			auto username(m_sites.add_object<ui::textbox>("home",
														  tbsize,
														  vec2f{(m_sites.bounds().width - tbsize.x) / 2.f, logo_shape->get().getPosition().y + 180.f},
														  font,
														  "Username"));
			auto password(m_sites.add_object<ui::textbox>("home",
														  tbsize,
														  vec2f{(m_sites.bounds().width - tbsize.x) / 2.f, logo_shape->get().getPosition().y + 220.f},
														  font,
														  "Password"));
			default_textbox(*username);
			default_textbox(*password);
			password->setPasswordMode(true);

			// buttons
			const vec2f btnsize{100.f, 30.f};
			auto login(m_sites.add_object<ui::button>("home", btnsize, vec2f{password->getPosition().x, password->getPosition().y + 40.f}));
			login->setFont(font);
			login->setText("Login");

			auto register_acc(m_sites.add_object<ui::button>("home", btnsize, vec2f{password->getPosition().x + 150.f, password->getPosition().y + 40.f}));
			register_acc->setFont(font);
			register_acc->setText("Register");
			register_acc->on_clicked = [this]{m_overlay.mainmenu().gamehandler().popupmgr().template create_popup<popup_type::info>("Register a new account online at: http://");};

			default_button(*login);
			default_button(*register_acc);
		}
	};
}


#endif // RJ_GAME_MAIN_MENU_SITE_HOME_HPP

