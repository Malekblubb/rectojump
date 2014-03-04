//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_CORE_MAIN_MENU_ITEMS_HPP
#define RJ_CORE_MAIN_MENU_ITEMS_HPP


#include "basic_component.hpp"
#include "item.hpp"
#include <rectojump/core/render.hpp>

#include <mlk/containers/container_utl.h>
#include <mlk/signals_slots/slot.h>


namespace rj
{
	template<typename Main_Menu>
	class items : public basic_component
	{
		Main_Menu& m_mainmenu;
		rndr& m_render;

		static constexpr float m_spacing{50.f};
		std::map<item, sf::Text> m_menuitems;
		std::size_t m_current_index{0};

		mlk::event_delegates<item_id> m_events;

	public:
		items(Main_Menu& mm) :
			basic_component{mm.gamehandler().rendermgr(), mm.get_font(), mm.get_center()},
			m_mainmenu{mm},
			m_render{mm.gamehandler().rendermgr()}
		{ }

		void update(dur)
		{
			for(auto& a : m_menuitems)
			{
				if(a.first.index == m_current_index)
					a.second.setColor(m_mainmenu.get_act_fontcolor());
				else
					a.second.setColor(m_mainmenu.get_def_fontcolor());
			}
		}

		void render()
		{for(auto& a : m_menuitems) m_render(a.second);}

		void add_item(const item_id& id, const std::string& text)
		{
			m_menuitems.emplace(item{id, this->num_items()}, sf::Text{text, m_font});
			this->recalc_positions();
		}

		void call_current_event() override
		{
			mlk::cnt::map_first_foreach(m_menuitems,
			[this](const auto& i)
			{
				if(i.index == m_current_index)
					m_events[i.id]();
			});
		}

		template<typename Func>
		void on_event(const item_id& id, Func&& f)
		{
			m_events[id] += f;
		}

		void on_key_up() override
		{
			if(m_current_index <= 0)
				m_current_index = this->max_index();
			else
				--m_current_index;
		}

		void on_key_down() override
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
				auto tmp(a.second.getGlobalBounds());
				rect.height += tmp.height;
				rect.width += tmp.width;
			}

			auto num(0);
			for(auto& a : m_menuitems)
			{
				auto tmp_rect(a.second.getGlobalBounds());
				a.second.setOrigin(tmp_rect.width / 2.f, tmp_rect.height / 2.f);
				a.second.setPosition(m_center.x, (m_center.y - rect.height / 2) + (num * m_spacing));
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
