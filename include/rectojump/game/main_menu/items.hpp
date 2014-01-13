//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_CORE_MAIN_MENU_ITEMS_HPP
#define RJ_CORE_MAIN_MENU_ITEMS_HPP


#include "basic_component.hpp"
#include "item.hpp"
#include <rectojump/core/render.hpp>


namespace rj
{
	class game;

	class items : protected basic_component
	{
		const sf::Color& m_def_fontcolor;
		const sf::Color& m_act_fontcolor;

		static constexpr float m_spacing{50.f};
		std::map<item, sf::Text> m_menuitems;
		std::size_t m_current_index{0};

	public:
		items(game& g, const sf::Font& font, const vec2f& center, const sf::Color& def, const sf::Color& act) :
			basic_component{g, font, center},
			m_def_fontcolor{def},
			m_act_fontcolor{act}
		{ }

		void update(dur)
		{
			for(auto& a : m_menuitems)
			{
				if(a.first.index == m_current_index)
					a.second.setColor(m_act_fontcolor);
				else
					a.second.setColor(m_def_fontcolor);
			}
		}

		void render()
		{for(auto& a : m_menuitems) render::render_object(m_game, a.second);}

		void add_item(const item_id& id, const std::string& text)
		{
			m_menuitems.emplace(item{id, this->num_items()}, sf::Text{text, m_font});
			this->recalc_positions();
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

		item get_current_selected() const
		{
			for(auto& a : m_menuitems)
				if(a.first.index == m_current_index)
					return a.first;
			return {"", 0};
		}

	private:
		void recalc_positions()
		{
			// calc positions
			sf::FloatRect rect{0.f, 0.f, 0.f, 0.f};
			for(auto& a : m_menuitems)
			{
				auto tmp(a.second.getLocalBounds());
				rect.height += tmp.height;
				rect.width += tmp.width;
			}

			auto num(0);
			for(auto& a : m_menuitems)
			{
				auto tmp_rect(a.second.getLocalBounds());
				a.second.setOrigin(tmp_rect.width / 2.f, tmp_rect.height / 2.f);
				a.second.setPosition(m_center.x, (m_center.y - rect.height / 2) + (num * m_spacing));
				a.second.setColor(m_def_fontcolor);
				++num;
			}
		}

		std::size_t num_items() const noexcept
		{return m_menuitems.size();}

		std::size_t max_index() const noexcept
		{return this->num_items() - 1;}
	};
}


#endif // RJ_CORE_MAIN_MENU_ITEMS_HPP
