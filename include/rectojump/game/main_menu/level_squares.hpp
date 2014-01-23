//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_CORE_MAIN_MENU_LEVEL_SQUARES_HPP
#define RJ_CORE_MAIN_MENU_LEVEL_SQUARES_HPP


#include "basic_component.hpp"
#include "level_square.hpp"
#include <rectojump/core/render.hpp>
#include <rectojump/shared/level_manager/level_manager.hpp>

#include <mlk/signals_slots/slot.h>


namespace rj
{
	enum class scroll_dir : char
	{up, down, none};

	template<typename Main_Menu>
	class level_squares : public basic_component
	{
		Main_Menu& m_mainmenu;

		const vec2f m_square_size{100, 100};
		std::vector<level_square<Main_Menu>> m_squares;
		std::size_t m_current_index{0};

		scroll_dir m_sdir{scroll_dir::none};
		float m_scrollstep{1.5f};

	public:
		mlk::slot<const level_id&> on_level_load;

		level_squares(Main_Menu& mm) :
			basic_component{mm.get_gamehandler().get_game(), mm.get_font(), mm.get_center()},
			m_mainmenu{mm}
		{this->init();}

		void update(dur duration)
		{
			if(m_squares[m_current_index].get_position().y > m_center.y + 5.f) // 5: add some 'space'
				this->scroll(scroll_dir::down);
			else if(m_squares[m_current_index].get_position().y < m_center.y - 5.f)
				this->scroll(scroll_dir::up);
			else
				this->scroll_stop();

			for(auto& a : m_squares)
			{
				a.update(duration);
				a.deactivate();

				if(m_sdir == scroll_dir::down) a.move({0.f, -m_scrollstep});
				else if(m_sdir == scroll_dir::up) a.move({0.f, m_scrollstep});
			}

			m_squares[m_current_index].activate();
		}

		void render()
		{
			for(auto& a : m_squares)
				a.render();
		}

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
		{on_level_load(m_squares[m_current_index].get_id());}

	private:
		void init()
		{
			auto pos_y(0.f);

			for(auto& a : m_mainmenu.get_gamehandler().get_levelmgr().get_levels())
			{
				auto& id(a.first);
				auto& lv(a.second);
				auto& inf(lv.info);

				m_squares.emplace_back(m_mainmenu, vec2f{m_center.x, pos_y}, m_square_size,
									   "Name", mlk::stl_string::str_format("Creator: %%\nDate: %%", inf.creator_name, inf.creation_date), id);
				pos_y += m_squares.back().get_height();
			}
		}

		std::size_t num_items() const noexcept
		{return m_squares.size();}

		std::size_t max_index() const noexcept
		{return this->num_items() - 1;}
	};
}


#endif // RJ_CORE_MAIN_MENU_LEVEL_SQUARES_HPP
