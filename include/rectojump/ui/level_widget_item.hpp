//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_UI_LEVEL_WIDGET_ITEM_HPP
#define RJ_UI_LEVEL_WIDGET_ITEM_HPP


#include "widget.hpp"
#include <rectojump/global/config_settings.hpp>
#include <rectojump/shared/level_manager/level.hpp>
#include <rectojump/shared/input.hpp>


namespace rj
{
	namespace ui
	{
		class level_widget_item : public widget
		{
			sf::RectangleShape m_background{{100.f, 100.f}};
			sf::RectangleShape m_preview;
			sf::Text m_level_name;
			sf::Text m_creator_date;
//			sf::Text m_creator_text; // TODO: impl

			sf::Color m_color_active{to_rgb("#f15ede", 80)}, m_color_deactive{to_rgb("#bdbdbd")};

			bool m_active{false};

		public:
			level_widget_item(const level& lv, const sf::Font& font, float width) :
				m_level_name{"", font, settings::text_size() + 3},
				m_creator_date{"", font, settings::text_size()}
			{
				if(!lv.is_valid())
					return;

				m_preview.setPosition(10.f, 10.f);
				m_preview.setSize({75.f, 75.f});
				m_preview.setFillColor(sf::Color::Black);

				m_level_name.setString(lv.info.level_name);
				m_level_name.setPosition(110.f, 10.f);
				m_creator_date.setString(lv.info.creator_name + "\n" + lv.info.creation_date);
				m_creator_date.setPosition(110.f, m_level_name.getGlobalBounds().top + m_level_name.getGlobalBounds().height);

				m_background.setFillColor(m_color_deactive);
				m_background.setSize({width, 100.f});
			}

			void update(dur) override
			{
				if(inp::was_real_mousepress_left())
				{
					if(inp::get_mousebounds<true>().intersects(m_background.getGlobalBounds()))
					{
						m_active = true;
						m_background.setFillColor(m_color_active);
					}
					else
					{
						m_active = false;
						m_background.setFillColor(m_color_deactive);
					}
				}
			}

			void move(const vec2f& offset)
			{
				m_background.move(offset);
				m_preview.move(offset);
				m_level_name.move(offset);
				m_creator_date.move(offset);
			}

			const vec2f& size() const noexcept
			{return m_background.getSize();}

			sf::FloatRect bounds() const noexcept
			{return m_background.getGlobalBounds();}

			float pos_y_h() const noexcept
			{return m_background.getGlobalBounds().top + m_background.getGlobalBounds().height;}

		private:
			void draw(sf::RenderTarget& target, sf::RenderStates states) const override
			{
				target.draw(m_background, states);
				target.draw(m_preview, states);
				target.draw(m_level_name, states);
				target.draw(m_creator_date, states);
			}
		};
	}
}


#endif // RJ_UI_LEVEL_WIDGET_ITEM_HPP
