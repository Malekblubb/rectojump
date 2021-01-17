//
// Copyright (c) 2013-2021 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_GAME_MENU_GAME_MENU_HPP
#define RJ_GAME_GAME_MENU_GAME_MENU_HPP

#include <map>
#include <rectojump/global/common.hpp>
#include <rectojump/global/config_settings.hpp>
#include <rectojump/ui/button.hpp>

namespace rj
{
	template <typename Game_Handler>
	class game_menu
	{
		Game_Handler& m_gamehandler;
		rndr& m_render;

		static constexpr float m_width{400.f}, m_height{400.f},
			m_btn_width{m_width - 20.f}, m_btn_height{30.f};
		sf::RectangleShape m_main_shape;
		sf::Font m_font;
		sf::Text m_pausetext;

		using button_id = std::string;
		std::map<button_id, ui::button> m_menu_entries;

	public:
		game_menu(Game_Handler& gh)
			: m_gamehandler{gh},
			  m_render{gh.rendermgr()},
			  m_font{gh.datamgr().template get_as<sf::Font>(
				  settings::rj_font())}// TODO: this should be gh.datastore()
		{
			this->init();
		}

		void update(dur duration)
		{
			for(auto& e : m_menu_entries) e.second.update(duration);
		}

		void render() { m_render(m_main_shape, m_pausetext, m_menu_entries); }

	private:
		void init()
		{
			// background shape
			m_main_shape.setSize({m_width, m_height});
			m_main_shape.setOutlineThickness(2.f);
			m_main_shape.setOutlineColor(settings::get_color_light());
			m_main_shape.setFillColor(sf::Color::Black);
			m_main_shape.setOrigin(m_width / 2.f, m_height / 2.f);
			m_main_shape.setPosition(settings::get_window_size<vec2f>() / 2.f);

			// pause text
			m_pausetext.setFont(m_font);
			m_pausetext.setString("PAUSE");
			m_pausetext.setFillColor(settings::get_color_dark());
			m_pausetext.setOutlineThickness(1.f);
			m_pausetext.setOutlineColor(settings::get_color_default_light());
			auto bounds{m_pausetext.getGlobalBounds()};
			m_pausetext.setOrigin(vec2f{bounds.width, bounds.height} / 2.f);
			m_pausetext.setPosition(
				{settings::get_window_size<vec2f>().x / 2,
				 m_main_shape.getGlobalBounds().top + bounds.height});

			// buttons
			auto spacing{15.f}, start{100.f};
			this->add_entry(
				"btn_back_to_game", "Back to Game",
				[this] { m_gamehandler.exit_game_menu(); }, start);

			this->add_entry(
				"btn_back_main_menu", "Back to Main Menu",
				[this] {
					// TODO: add asking dialoges
					m_gamehandler.switch_to_main_menu();
				},
				start + m_btn_height);
			this->add_entry(
				"btn_restart", "Restart Level",
				[this] { m_gamehandler.restart_level(); },
				start + m_btn_height * 2);
			this->add_entry(
				"btn_exit", "Exit Game", [this] { m_gamehandler.exit(); },
				start + (m_btn_height + spacing) * 3);
		}

		template <typename Fun>
		auto& add_entry(const button_id& id, const std::string text, Fun f,
						float pos_y)
		{
			m_menu_entries.emplace(id, ui::button{});
			auto& entry{m_menu_entries[id]};
			default_button(entry);
			entry.setSize({m_btn_width, m_btn_height});
			entry.setOrigin(entry.getSize() / 2.f);
			entry.setPosition({settings::get_window_size<vec2f>().x / 2.f,
							   m_main_shape.getGlobalBounds().top + pos_y});
			entry.setFont(m_gamehandler.datastore().template get<sf::Font>(
				"Ubuntu-R.ttf"));
			entry.setText(text);
			entry.on_clicked = f;
			return entry;
		}
	};
}// namespace rj

#endif// RJ_GAME_GAME_MENU_GAME_MENU_HPP
