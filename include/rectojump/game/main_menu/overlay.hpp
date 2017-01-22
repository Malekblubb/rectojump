//
// Copyright (c) 2013-2017 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_MAIN_MENU_OVERLAY_HPP
#define RJ_GAME_MAIN_MENU_OVERLAY_HPP


#include "site_credits.hpp"
#include "site_editor.hpp"
#include "site_home.hpp"
#include "site_inventar.hpp"
#include "site_levels.hpp"
#include "site_play.hpp"
#include "site_scores.hpp"
#include "site_settings.hpp"
#include <rectojump/global/config_settings.hpp>
#include <rectojump/ui/connected_buttons.hpp>
#include <rectojump/ui/stacked_widget.hpp>


namespace rj
{
    template<typename Main_Menu, typename Game_Handler>
	class overlay
	{        
		Main_Menu& m_mainmenu;
		rndr& m_render;
		data_store_type& m_datastore;

		sf::RectangleShape m_logo;
		sf::RectangleShape m_blubber;
		sf::Text m_infotext;

		sf::RectangleShape m_main_border;
		sf::RectangleShape m_menu_bar;
		ui::connected_buttons m_menu_buttons;
		const vec2f m_btn_size{110.f, 40.f};

		// stacked widget (sites)
		ui::stacked_widget m_sites;

		// encapsulate sites construction
		site_home<overlay> m_sitehome;
		site_play<overlay> m_siteplay;
		site_inventar<overlay> m_siteinventar;
        site_editor<overlay, Game_Handler> m_siteeditor;
		site_levels<overlay> m_sitelevels;
		site_scores<overlay> m_sitescores;
		site_settings<overlay> m_sitesettings;
		site_credits<overlay> m_sitecredits;

	public:
		overlay(Main_Menu& mm) :
			m_mainmenu{mm},
			m_render{mm.gamehandler().rendermgr()},
			m_datastore{mm.gamehandler().datastore()},
			m_infotext{"", m_datastore.template get<sf::Font>(settings::text_font())},
			m_sites{mm.gamehandler().gamewindow()},
			m_sitehome{*this},
			m_siteplay{*this},
			m_siteinventar{*this},
			m_siteeditor{*this},
			m_sitelevels{*this},
			m_sitescores{*this},
			m_sitesettings{*this},
			m_sitecredits{*this}
		{this->init();}

		void update(dur duration)
		{
			m_menu_buttons.update(duration);
			m_sites.update(duration);
		}

		void render()
		{
			// render border, logo, buttons...
			m_render(m_main_border, m_menu_bar, m_menu_buttons, m_logo, m_blubber, m_infotext);

			// render sites
			m_sites.activate_current_cam();
			m_render(m_sites);
		}

		auto& mainmenu() noexcept
		{return m_mainmenu;}

		auto& sites() noexcept
		{return m_sites;}

	private:
		void init()
		{
			this->init_main_menu();
			this->init_sites();
		}

		void init_main_menu()
		{
			const auto size(settings::get_window_size<vec2f>());

			// logo
			m_logo.setSize({506.f, 100.f});
			m_logo.setPosition(40.f, 0.f);
			m_logo.setTexture(&m_datastore.template get<sf::Texture>("logo.png"));

			// blubber
			m_blubber.setSize({71.f, 100.f});
			m_blubber.setPosition(size.x - 40.f, 0.f);
			m_blubber.setOrigin(71.f, 0.f);
			m_blubber.setTexture(&m_datastore.template get<sf::Texture>("blubber.png"));

			// border
			m_main_border.setSize({size.x - 80, size.y - 140});
			m_main_border.setPosition(40, 100);
			m_main_border.setOutlineThickness(4.f);
			m_main_border.setOutlineColor(to_rgb("#bf35ad"));
			m_main_border.setFillColor(to_rgb("#e3e3e3"));

			// info text
			m_infotext.setString("(c) 2013-2017 Christoph Malek");
			m_infotext.setCharacterSize(settings::text_size());
			m_infotext.setFillColor(to_rgb("#e3e3e3"));
			m_infotext.setPosition(m_main_border.getPosition().x, m_main_border.getGlobalBounds().top + m_main_border.getGlobalBounds().height + 10.f);

			// menu bar
			m_menu_bar.setSize({size.x - 80, 40});
			m_menu_bar.setPosition(40, 140);
			m_menu_bar.setOutlineThickness(2.f);
			m_menu_bar.setOutlineColor(to_rgb("#f15ede"));
			m_menu_bar.setFillColor({255, 255, 255, 255});

			// menu buttons
			m_menu_buttons.on_active_button = [](auto& btn){btn->setFillColor(to_rgb("#f15ede"));};
			m_menu_buttons.on_inactive_button = [](auto& btn){btn->setFillColor(to_rgb("#cecece"));};

			const auto next((size.x - 80.f) / 9);
			this->add_menu_button(next, 1.f, "home.png");
			this->add_menu_button(next, 2.f, "play.png");
			this->add_menu_button(next, 3.f, "inventar.png");
			this->add_menu_button(next, 4.f, "editor.png");
			this->add_menu_button(next, 5.f, "levels.png");
			this->add_menu_button(next, 6.f, "scores.png");
			this->add_menu_button(next, 7.f, "settings.png");
			this->add_menu_button(next, 8.f, "credits.png");
		}

		void init_sites()
		{
			const auto size(settings::get_window_size<vec2f>());

			// set default properties
			m_sites.set_size({size.x - 80.f, size.y - 220.f});
			m_sites.set_pos({40.f, 180.f});

			// add sites
			m_sites.add_site("home");
			m_sites.add_site("play");
			m_sites.add_site("inventar");
			m_sites.add_site("editor");
			m_sites.add_site("levels");
			m_sites.add_site("scores");
			m_sites.add_site("settings");
			m_sites.add_site("credits");

			// construct sites content
			m_sitehome.construct();
			m_siteplay.construct();
			m_siteinventar.construct();
			m_siteeditor.construct();
			m_sitelevels.construct();
			m_sitescores.construct();
			m_sitesettings.construct();
			m_sitecredits.construct();

			// switch to home-site by default
			m_sites.switch_site("home");
		}

		void add_menu_button(float next, float btn_nr, const std::string& texture)
		{
			auto site_name(texture);
			mlk::stl_string::erase_all(".png", site_name);

			auto btn(m_menu_buttons.add_button_event<ui::button>([this, site_name]{m_sites.switch_site(site_name);}, m_btn_size, vec2f{next * btn_nr, 140}));
			btn->setTexture(&m_datastore.template get<sf::Texture>(texture));
			btn->setOrigin({m_btn_size.x / 2.f, 0});
		}
	};
}


#endif // RJ_GAME_MAIN_MENU_OVERLAY_HPP

