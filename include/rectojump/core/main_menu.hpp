//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_CORE_MAIN_MENU_HPP
#define RJ_CORE_MAIN_MENU_HPP


#include "game_window.hpp"
#include "render.hpp"
#include <rectojump/global/common.hpp>
#include <rectojump/shared/data_manager.hpp>
#include <rectojump/shared/utils.hpp>

#include <mlk/graphics/color.h>
#include <mlk/signals_slots/slot.h>

#include <SFML/Graphics.hpp>


namespace rj
{
	class game;

	class main_menu
	{
		game& m_game;
		game_window& m_gamewindow;
		data_manager& m_datamanager;

		sf::Font m_font{m_datamanager.get_as<sf::Font>("Fipps-Regular.otf")};
		const sf::Color m_default_fontcolor{"#797979"_rgb};
		const sf::Color m_active_fontcolor{"#f15ede"_rgb};
		std::vector<sf::Text> m_menuitems;
		sf::RectangleShape m_background;

		static constexpr float m_spacing{50.f};
		vec2f m_middle{static_cast<vec2f>(m_gamewindow.get_size()) / 2.f};

		std::size_t m_current_index{0};

	public:
		main_menu(game& g, game_window& gw, data_manager& dm) :
			m_game{g},
			m_gamewindow{gw},
			m_datamanager{dm}
		{this->init();}


		void update(dur duration)
		{
			for(auto& a : m_menuitems)
				a.setColor(m_default_fontcolor);

			m_menuitems.at(m_current_index).setColor(m_active_fontcolor);
		}

		void render()
		{
			render::render_object(m_game, m_background);
			for(auto& a : m_menuitems)
				render::render_object(m_game, a);
		}

	private:
		void init()
		{
			on_key_pressed(key::Up) += [this]{this->on_key_up();};
			on_keys_pressed(key::Down) += [this]{this->on_key_down();};

			// add items
			m_menuitems.emplace_back(sf::Text{"Play", m_font});
			m_menuitems.emplace_back(sf::Text{"Options", m_font});
			m_menuitems.emplace_back(sf::Text{"Credits", m_font});

			// calc positions
			sf::FloatRect rect{0.f, 0.f, 0.f, 0.f};
			for(auto& a : m_menuitems)
			{
				auto tmp(a.getLocalBounds());
				rect.height += tmp.height;
				rect.width += tmp.width;
			}

			auto num(0);
			for(auto& a : m_menuitems)
			{
				auto tmp_rect(a.getLocalBounds());
				a.setOrigin(tmp_rect.width / 2.f, tmp_rect.height / 2.f);
				a.setPosition(m_middle.x, (m_middle.y - rect.height / 2) + (num * m_spacing));
				a.setColor(m_default_fontcolor);
				++num;
			}

			// background
			m_background.setSize(vec2f{m_gamewindow.get_size()});
			m_background.setPosition({0.f, 0.f});
			m_background.setFillColor("#e3e3e3"_rgb);
		}

		std::size_t num_items() const noexcept
		{return m_menuitems.size();}

		std::size_t max_index() const noexcept
		{return this->num_items() - 1;}

		void on_key_up()
		{
			if(m_current_index <= 0)
				m_current_index = this->max_index();
			else
				--m_current_index;
		}

		void on_key_down()
		{
			if(m_current_index >= this->max_index())
				m_current_index = 0;
			else
				++m_current_index;
		}
	};
}


#endif // RJ_CORE_MAIN_MENU_HPP
