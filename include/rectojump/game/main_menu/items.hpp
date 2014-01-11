//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_CORE_MAIN_MENU_ITEMS_HPP
#define RJ_CORE_MAIN_MENU_ITEMS_HPP


#include "menu_component.hpp"


namespace rj
{
	class game;

	class items : public menu_component
	{
		const sf::Color& m_def_fontcolor;
		const sf::Color& m_act_fontcolor;

		static constexpr float m_spacing{50.f};
		std::vector<sf::Text> m_menuitems;
		std::size_t m_current_index{0};

	public:
		items(game& g, const sf::Font& font, const vec2f& center, const sf::Color& def, const sf::Color& act) :
			menu_component{g, font, center},
			m_def_fontcolor{def},
			m_act_fontcolor{act}
		{this->init();}

		void update(dur)
		{
			for(auto& a : m_menuitems)
				a.setColor(m_def_fontcolor);
			m_menuitems.at(m_current_index).setColor(m_act_fontcolor);
		}

		void render()
		{
			for(auto& a : m_menuitems) render::render_object(m_game, a);
		}

	private:
		void init() override
		{
			on_key_pressed(key::Up) += [this]{this->on_key_up();};
			on_keys_pressed(key::Down) += [this]{this->on_key_down();};

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
				a.setPosition(m_center.x, (m_center.y - rect.height / 2) + (num * m_spacing));
				a.setColor(m_def_fontcolor);
				++num;
			}
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


#endif // RJ_CORE_MAIN_MENU_ITEMS_HPP
