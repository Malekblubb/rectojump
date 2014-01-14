//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_CORE_MAIN_MENU_TITLE_HPP
#define RJ_CORE_MAIN_MENU_TITLE_HPP


#include "basic_component.hpp"
#include <rectojump/core/render.hpp>


namespace rj
{
	class game;

	enum class direction : char
	{forward, back};

	class title : public basic_component
	{
		sf::Text m_title{"Recto Jump", m_font, 50};
		std::vector<sf::Color> m_title_colors;
		std::size_t m_current_colorindex{0};
		direction m_direction{direction::forward};

	public:
		title(game& g, const sf::Font& font, const vec2f& center) :
			basic_component{g, font, center}
		{this->init();}

		void update(dur)
		{
			m_direction == direction::forward ? ++m_current_colorindex : --m_current_colorindex;

			if(m_current_colorindex == m_title_colors.size() - 1)
				m_direction = direction::back;
			else if(m_current_colorindex == 0)
				m_direction = direction::forward;

			m_title.setColor(m_title_colors[m_current_colorindex]);
		}

		void render()
		{render::render_object(m_game, m_title);}

		void set_text(const std::string& text) noexcept
		{m_title.setString(text); this->set_pos();}

	private:
		void init() noexcept
		{
			// pos
			this->set_pos();

			// colors
			for(std::uint8_t r{255}, b{200}; r > 123; --r, --b)
				m_title_colors.emplace_back(r, 0, b);
		}

		void set_pos() noexcept
		{
			m_title.setOrigin(m_title.getGlobalBounds().width / 2.f, m_title.getGlobalBounds().height / 2.f);
			m_title.setPosition(m_center.x, 150);
		}
	};
}


#endif // RJ_CORE_MAIN_MENU_TITLE_HPP
