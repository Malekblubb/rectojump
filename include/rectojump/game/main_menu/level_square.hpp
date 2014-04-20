//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_CORE_MAIN_MENU_LEVEL_SQUARE_HPP
#define RJ_CORE_MAIN_MENU_LEVEL_SQUARE_HPP


#include <rectojump/core/render.hpp>
#include <rectojump/global/common.hpp>
#include <rectojump/shared/level_manager/level_manager.hpp>


namespace rj
{
	template<typename Main_Menu>
	class level_square
	{
		Main_Menu& m_mainmenu;
		rndr& m_render;

		sf::RectangleShape m_shape;
		sf::Text m_name;
		sf::Text m_text;
		static constexpr float m_text_offset{60.f};
		static constexpr float m_spacing{20.f};

		level_id m_id;
		bool m_active{false};

	public:
		level_square(Main_Menu& mm, const vec2f& pos, const vec2f& size, const std::string& name, const std::string& text, const level_id& id) :
			m_mainmenu{mm},
			m_render{mm.gamehandler().rendermgr()},
			m_shape{size},
			m_name{name, mm.get_font(), 30},
			m_text{text, mm.get_font(), 15},
			m_id{id}
		{
			// shape
			m_shape.setOrigin(size / 2.f);
			m_shape.setPosition({pos.x - m_text_offset, pos.y});
			m_shape.setOutlineThickness(2.f);
			auto shape_bounds(m_shape.getGlobalBounds());

			// name text
			m_name.setPosition({shape_bounds.left + size.x + m_spacing, shape_bounds.top + m_spacing});
			auto name_bounds(m_name.getGlobalBounds());

			// info text
			auto text_bounds(m_text.getGlobalBounds());
			m_text.setPosition({shape_bounds.left + size.x + m_spacing, name_bounds.top + name_bounds.height + m_spacing});
		}

		void update(dur duration)
		{
			if(m_active)
			{
				m_shape.setOutlineColor(m_mainmenu.get_act_fontcolor());
				m_name.setColor(m_mainmenu.get_act_fontcolor());
				m_text.setColor(m_mainmenu.get_act_fontcolor());
			}
			else
			{
				m_shape.setOutlineColor(m_mainmenu.get_def_fontcolor());
				m_name.setColor(m_mainmenu.get_def_fontcolor());
				m_text.setColor(m_mainmenu.get_def_fontcolor());
			}
		}

		void render()
		{m_render(m_shape, m_name, m_text);}

		void move(const vec2f& offset) noexcept
		{
			m_shape.move(offset);
			m_name.move(offset);
			m_text.move(offset);
		}

		void activate() noexcept
		{m_active = true;}

		void deactivate() noexcept
		{m_active = false;}

		const vec2f& get_position() const noexcept
		{return m_shape.getPosition();}

		float get_height() const noexcept
		{return m_shape.getSize().y + m_name.getGlobalBounds().height + m_text.getGlobalBounds().height;}

		const sf::FloatRect get_bounds()
		{
			auto shapebounds(m_shape.getGlobalBounds());
			return {shapebounds.left, shapebounds.top, m_shape.getSize().x + m_text.getGlobalBounds().width, this->get_height()};
		}

		const level_id& get_id() const noexcept
		{return m_id;}
	};
}


#endif // RJ_CORE_MAIN_MENU_LEVEL_SQUARE_HPP
