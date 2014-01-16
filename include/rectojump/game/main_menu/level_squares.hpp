//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_CORE_MAIN_MENU_LEVEL_SQUARES_HPP
#define RJ_CORE_MAIN_MENU_LEVEL_SQUARES_HPP


#include "basic_component.hpp"
#include <rectojump/core/render.hpp>
#include <rectojump/shared/level_manager/level_manager.hpp>

#include <mlk/signals_slots/slot.h>


namespace rj
{
	enum class scroll_dir : char
	{up, down, none};

	class level_squares : public basic_component
	{
		const sf::Color& m_def_fontcolor;
		const sf::Color& m_act_fontcolor;

		const vec2f m_size{100, 100};
		std::vector<sf::RectangleShape> m_shapes;
		std::size_t m_current_index{0};

		scroll_dir m_sdir{scroll_dir::none};
		float m_scrollstep{1.5f};

	public:
		mlk::slot<const level_id&> on_level_load;

		level_squares(game& g, const sf::Font& font, const vec2f& center, const sf::Color& def, const sf::Color& act) :
			basic_component{g, font, center},
			m_def_fontcolor{def},
			m_act_fontcolor{act}
		{this->init();}

		void update(dur duration)
		{
			if(m_shapes[m_current_index].getPosition().y > m_center.y)
				this->scroll(scroll_dir::down);
			else if(m_shapes[m_current_index].getPosition().y < m_center.y)
				this->scroll(scroll_dir::up);
			else
				this->scroll_stop();

			if(m_sdir == scroll_dir::down) for(auto& a : m_shapes) a.move(0.f, -m_scrollstep);
			else if(m_sdir == scroll_dir::up) for(auto& a : m_shapes) a.move(0.f, m_scrollstep);

			for(auto& a : m_shapes)
				a.setOutlineColor(m_def_fontcolor);
			m_shapes[m_current_index].setOutlineColor(m_act_fontcolor);
		}

		void render()
		{for(auto& a : m_shapes) rndr::ro(m_game, a);}

		void scroll(scroll_dir dir) noexcept
		{m_sdir = dir;}

		void scroll_stop() noexcept
		{m_sdir = scroll_dir::none;}

		void on_key_up() noexcept
		{
			if(m_current_index <= 0)
				m_current_index = this->max_index();
			else
				--m_current_index;
		}

		void on_key_down() noexcept
		{
			if(m_current_index >= this->max_index())
				m_current_index = 0;
			else
				++m_current_index;
		}

		void call_current_event() override
		{on_level_load("");}

	private:
		void init()
		{
			auto pos_y(150.f);

			for(/*auto& a : m_lvmgr.levels()*/auto i(0);i < 10; ++i, pos_y += 150)
			{
				m_shapes.emplace_back(m_size);
				auto& back(m_shapes.back());
				back.setOrigin(m_size / 2.f);
				back.setOutlineThickness(2.f);
				back.setOutlineColor({});
				back.setPosition(m_center.x, pos_y);
			}
		}

		std::size_t num_items() const noexcept
		{return m_shapes.size();}

		std::size_t max_index() const noexcept
		{return this->num_items() - 1;}
	};
}


#endif // RJ_CORE_MAIN_MENU_LEVEL_SQUARES_HPP
