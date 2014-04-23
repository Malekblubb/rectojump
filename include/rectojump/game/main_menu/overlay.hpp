//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_MAIN_MENU_OVERLAY_HPP
#define RJ_GAME_MAIN_MENU_OVERLAY_HPP


#include <rectojump/ui/connected_buttons.hpp>
#include <rectojump/ui/stacked_widget.hpp>


namespace rj
{
	template<typename Game_Handler>
	class overlay
	{
		Game_Handler& m_gamehandler;
		rndr& m_render;
		typename Game_Handler::data_store_type& m_datastore;

		sf::RectangleShape m_logo;
		sf::RectangleShape m_blubber;

		sf::RectangleShape m_main_border;
		sf::RectangleShape m_menu_bar;
		ui::connected_buttons m_menu_buttons;
		const vec2f m_btn_size{110.f, 40.f};

		// stacked widget (sites)
		ui::stacked_widget m_sites;

	public:
		overlay(Game_Handler& gh) :
			m_gamehandler{gh},
			m_render{gh.rendermgr()},
			m_datastore{gh.datastore()},
			m_sites{gh.gamewindow()}
		{this->init();}

		void update(dur duration)
		{
			m_menu_buttons.update(duration);
			m_sites.update(duration);
		}

		void render()
		{
			// render border, logo, buttons...
			m_render(m_menu_bar, m_main_border, m_menu_buttons, m_logo, m_blubber);

			// render sites
			m_sites.activate_cam();
			m_render(m_sites);
		}

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
			m_main_border.setFillColor({0, 0, 0, 0}); // transparent fillcolor

			// menu bar
			m_menu_bar.setSize({size.x - 80, 40});
			m_menu_bar.setPosition(40, 140);
			m_menu_bar.setOutlineThickness(2.f);
			m_menu_bar.setOutlineColor(to_rgb("#f15ede"));
			m_menu_bar.setFillColor({0, 0, 0, 0});

			// menu buttons
			m_menu_buttons.on_active_button = [](auto& btn){btn->set_color(to_rgb("#f15ede"));};
			m_menu_buttons.on_inactive_button = [](auto& btn){btn->set_color(to_rgb("#cecece"));};

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
			m_sites.set_size({size.x - 80.f, size.y - 180.f});
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

			// add sites content
			// site: home
			auto logo_shape(m_sites.add_object<sf::RectangleShape>("home", vec2f{128, 128}));
			logo_shape->setTexture(&m_datastore.template get<sf::Texture>("rj_logo.png"));
			logo_shape->setPosition({(m_sites.bounds().width - logo_shape->getSize().x) / 2.f, 50.f});


			// site: play
			m_sites.add_object<sf::RectangleShape>("play", vec2f{500, 500});


			// site: credits
			auto text(m_sites.add_object<sf::Text>("credits", "", m_datastore.template get<sf::Font>(glob::text_font), glob::text_size));
			text->setString("Programmer: Christoph Malek <>\n"
							"Graphics: -\n"
							"Icons: Adam Whitcroft <http://adamwhitcroft.com/batch/>\n\n"
							"Libs: SFML, zlib");
			text->setColor(to_rgb("#555555"));
			text->setPosition({(m_sites.bounds().width - text->getGlobalBounds().width) / 2.f, 150.f});


			// switch to home-site by default
			m_sites.switch_site("home");
		}

		void add_menu_button(float next, float btn_nr, const std::string& texture)
		{
			auto site_name(texture);
			mlk::stl_string::erase_all(".png", site_name);

			auto btn(m_menu_buttons.add_button_event<ui::button>([this, site_name]{m_sites.switch_site(site_name);}, m_btn_size, vec2f{next * btn_nr, 140}));
			btn->set_texture(&m_datastore.template get<sf::Texture>(texture));
			btn->set_origin({m_btn_size.x / 2.f, 0});
		}
	};
}


#endif // RJ_GAME_MAIN_MENU_OVERLAY_HPP

