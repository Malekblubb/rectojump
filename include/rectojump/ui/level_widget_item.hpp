//
// Copyright (c) 2013-2021 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_UI_LEVEL_WIDGET_ITEM_HPP
#define RJ_UI_LEVEL_WIDGET_ITEM_HPP

#include "button.hpp"
#include "widget.hpp"
#include <rectojump/game/components/gradient_rect.hpp>
#include <rectojump/global/config_settings.hpp>
#include <rectojump/shared/input.hpp>
#include <rectojump/shared/level_manager/level.hpp>

namespace rj
{
	namespace ui
	{
		class level_widget_item final : public widget
		{
			sf::RectangleShape m_background{{100.f, 100.f}};
			gradient_rect m_preview;
			sf::Text m_level_name;
			sf::Text m_creator_date;
			// sf::Text m_creator_text; // TODO: impl

			button m_play_button;
			button m_edit_button;

			sf::Color m_color_active{to_rgb("#f15ede", 80)},
				m_color_deactive{to_rgb("#bdbdbd")};

			bool m_active{false};

		public:
			level_widget_item(const level& lv, const sf::Font& font,
							  float width)
				: m_level_name{"", font, settings::text_size() + 3},
				  m_creator_date{"", font, settings::text_size()}
			{
				if(!lv.is_valid()) return;

				auto spacing{10.f};

				m_background.setFillColor(m_color_deactive);
				m_background.setSize({width, 100.f});

				m_preview.set_position({spacing, spacing});
				m_preview.set_size({75.f, 75.f});
				m_preview.set_startcolor(lv.background.startcolor());
				m_preview.set_endcolor(lv.background.endcolor());
				m_preview.set_gradient_points(lv.background.pointcount());

				m_level_name.setString(lv.info.level_name);
				m_level_name.setPosition(110.f, 10.f);
				m_creator_date.setString(lv.info.creator_name + "\n" +
										 lv.info.creation_date);
				m_creator_date.setPosition(
					110.f, m_level_name.getGlobalBounds().top +
							   m_level_name.getGlobalBounds().height);

				// buttons
				m_edit_button.setSize({100.f, 30.f});
				m_edit_button.setFont(font);
				m_edit_button.setText("Edit");
				m_edit_button.setOrigin(
					{m_edit_button.getSize().x, m_edit_button.getSize().y / 2});
				m_edit_button.setPosition(
					{width - spacing, m_background.getGlobalBounds().top +
										  m_background.getSize().y / 2});
				default_button(m_edit_button);
				// m_edit_button.on_clicked = []{}; set in game > main_menu >
				// site_play.hpp

				m_play_button.setSize({100.f, 30.f});
				m_play_button.setFont(font);
				m_play_button.setText("Play");
				m_play_button.setOrigin(
					{m_play_button.getSize().x, m_play_button.getSize().y / 2});
				m_play_button.setPosition(
					{m_edit_button.getGlobalBounds().left - spacing,
					 m_background.getGlobalBounds().top +
						 m_background.getSize().y / 2});
				default_button(m_play_button);
				// m_play_button.on_clicked = []{}; set in game > main_menu >
				// site_play.hpp
			}

			void update(dur duration) override
			{
				// TODO: this function creates lags
				if(inp::was_real_mousepress_left())
				{
					if(inp::get_mousebounds<true>().intersects(
						   m_background.getGlobalBounds()))
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

				// buttons
				m_play_button.update(duration);
				m_edit_button.update(duration);
			}

			void move(const vec2f& offset)
			{
				m_background.move(offset);
				m_preview.move(offset);
				m_level_name.move(offset);
				m_creator_date.move(offset);
				m_play_button.move(offset);
				m_edit_button.move(offset);
			}

			const vec2f& size() const noexcept
			{
				return m_background.getSize();
			}

			sf::FloatRect bounds() const noexcept
			{
				return m_background.getGlobalBounds();
			}

			float pos_y_h() const noexcept
			{
				return m_background.getGlobalBounds().top +
					   m_background.getGlobalBounds().height;
			}

			auto& on_play_clicked() { return m_play_button.on_clicked; }

			auto& on_edit_clicked() { return m_edit_button.on_clicked; }

		private:
			void draw(sf::RenderTarget& target,
					  sf::RenderStates states) const override
			{
				target.draw(m_background, states);
				target.draw(m_preview, states);
				target.draw(m_level_name, states);
				target.draw(m_creator_date, states);

				// buttons
				target.draw(m_play_button, states);
				target.draw(m_edit_button, states);
			}
		};
	}// namespace ui
}// namespace rj

#endif// RJ_UI_LEVEL_WIDGET_ITEM_HPP
