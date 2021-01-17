//
// Copyright (c) 2013-2021 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_MAIN_MENU_SITE_EDITOR_HPP
#define RJ_GAME_MAIN_MENU_SITE_EDITOR_HPP

#include <rectojump/game/popup_manager.hpp>
#include <rectojump/global/config_settings.hpp>
#include <rectojump/ui/button.hpp>
#include <rectojump/ui/sf_widget.hpp>
#include <rectojump/ui/stacked_widget.hpp>
#include <rectojump/ui/textbox.hpp>

namespace rj
{
	template <typename Overlay, typename Game_Handler>
	class site_editor
	{
		Overlay& m_overlay;
		data_store_type& m_datastore;
		ui::stacked_widget& m_sites;
		Game_Handler& m_gamehandler;

	public:
		site_editor(Overlay& ov)
			: m_overlay{ov},
			  m_datastore{ov.mainmenu().gamehandler().datastore()},
			  m_sites{ov.sites()},
			  m_gamehandler{ov.mainmenu().gamehandler()}
		{
		}

		void construct()
		{
			// buttons
			const vec2f btnsize{200.f, 100.f};
			const auto& font{m_datastore.get<sf::Font>(settings::text_font())};
			auto login{
				m_sites.add_object<ui::button>("editor", btnsize, vec2f{})};
			login->setPosition(
				vec2f{(m_sites.bounds().width / 2) - login->getSize().x / 2,
					  (m_sites.bounds().height / 2) - login->getSize().y / 2});
			login->setFont(font);
			login->setText("Go to editor");
			default_button(*login);
			login->setFontSize(18);
			login->on_clicked = [this] { m_gamehandler.switch_to_editor(); };
		}
	};
}// namespace rj

#endif// RJ_GAME_MAIN_MENU_SITE_EDITOR_HPP
