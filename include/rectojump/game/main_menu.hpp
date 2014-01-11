//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_CORE_MAIN_MENU_HPP
#define RJ_CORE_MAIN_MENU_HPP


#include <rectojump/core/game_window.hpp>
#include <rectojump/core/render.hpp>
#include <rectojump/global/common.hpp>
#include <rectojump/shared/data_manager.hpp>
#include <rectojump/shared/utils.hpp>

#include <mlk/graphics/color.h>
#include <mlk/signals_slots/slot.h>

#include <SFML/Graphics.hpp>

#include <rectojump/game/components/player.hpp>
#include <rectojump/game/factory.hpp>
#include <mlk/tools/random_utl.h>


namespace rj
{
	class game;

	enum item : std::size_t
	{play, options, credits, quit};

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
		sf::Texture m_background_texture{m_datamanager.get_as<sf::Texture>("menu_side.png")};

		static constexpr float m_spacing{50.f};
		vec2f m_middle{static_cast<vec2f>(m_gamewindow.get_size()) / 2.f};

		std::size_t m_current_index{0};
		mlk::event_delegates<std::size_t> m_events;

		factory::eptr<player> m_player{factory::create<player>(vec2f{m_middle.x, m_middle.y / 0.55f})};
		mlk::hrs_time_pnt m_last_jump{mlk::tm::time_pnt()};
		mlk::ullong m_jump_interval{1000};

	public:
		main_menu(game& g, game_window& gw, data_manager& dm) :
			m_game{g},
			m_gamewindow{gw},
			m_datamanager{dm}
		{this->init();}


		void update(dur duration)
		{
			this->update_menu();
			this->update_player(duration);
		}

		void render()
		{
			render::render_object(m_game, m_background);
			for(auto& a : m_menuitems)
				render::render_object(m_game, a);
			m_player->render();
		}

	private:
		void init()
		{
			this->setup_input();
			this->setup_events();
			this->setup_interface();
		}

		void setup_input()
		{
			on_key_pressed(key::Up) += [this]{this->on_key_up();};
			on_keys_pressed(key::Down) += [this]{this->on_key_down();};
			on_keys_pressed(key::Return);
		}

		void setup_events()
		{
			mlk::slot<> test{[]{std::cout<< "test " << std::endl;}};
			m_events.emplace(item::play, test);
		}

		void setup_interface()
		{
			// add items
			m_menuitems.emplace_back("Play", m_font);
			m_menuitems.emplace_back("Options", m_font);
			m_menuitems.emplace_back("Credits", m_font);
			m_menuitems.emplace_back("Quit", m_font);

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
			m_background.setTexture(&m_background_texture);

			// player
			m_player->init();
			m_player->set_game(&m_game);
			m_player->render_object().setFillColor(m_active_fontcolor);
		}

		void update_menu()
		{
			for(auto& a : m_menuitems)
				a.setColor(m_default_fontcolor);
			m_menuitems.at(m_current_index).setColor(m_active_fontcolor);
		}

		void update_player(dur duration)
		{
			if(mlk::tm::timed_out(m_last_jump, m_jump_interval))
			{
				simulate_keypress(key::Space);
				m_jump_interval = mlk::rnd<mlk::ullong>(500, 5000);
				m_last_jump = mlk::tm::time_pnt();
			}
			m_player->update(duration);
		}

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

		std::size_t num_items() const noexcept
		{return m_menuitems.size();}

		std::size_t max_index() const noexcept
		{return this->num_items() - 1;}
	};
}


#endif // RJ_CORE_MAIN_MENU_HPP
