//
// Copyright (c) 2013-2017 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_UI_BUTTON_HPP
#define RJ_UI_BUTTON_HPP

#include "widget.hpp"
#include <rectojump/global/common.hpp>
#include <rectojump/shared/input.hpp>

#include <mlk/signals_slots/slot.h>
#include <mlk/types/types.h>

namespace rj
{
	namespace ui
	{
		class button : public widget
		{
		protected:
			sf::RectangleShape m_shape;
			sf::Text m_text;
			bool m_hover{false};
			bool m_press{false};

		private:
			sf::RectangleShape m_restore_shape;

		public:
			mlk::slot<> on_clicked;
			mlk::slot<> on_hovered;

			button(const vec2f& size = {0.f, 0.f},
				   const vec2f& pos = {0.f, 0.f})
				: m_shape{size}
			{
				m_shape.setPosition(pos);
				this->init_base();
			}

			button(const vec2f& size, const std::string& text,
				   const sf::Font& font, const sf::Color& fontcolor = {})
				: button{size}
			{
				m_text.setString(text);
				m_text.setFont(font);
				m_text.setFillColor(fontcolor);
				m_text.setCharacterSize(0);
				this->calculate_textpos();
			}

			void update(dur) override
			{
				// check states
				if(m_hover)
					this->on_hover();
				else
					this->on_hover_end();

				if(m_press)
					this->on_press();
				else
					this->on_press_end();

				// collision
				auto bounds{m_shape.getGlobalBounds()};
				auto mousebounds{inp::get_mousebounds<true>()};

				if(bounds.intersects(mousebounds))
					m_hover = true;
				else
					m_hover = false;

				if(m_hover)
				{
					if(inp::is_btn_pressed(btn::Left))
						m_press = true;
					else
						m_press = false;

					if(inp::was_real_mousepress_left()) this->on_clicked();
				}
				else
					m_press = false;
			}

			void setText(const std::string& text) noexcept
			{
				m_text.setString(text);
				this->calculate_textpos();
			}

			void setFont(const sf::Font& font) noexcept
			{
				m_text.setFont(font);
				this->calculate_textpos();
			}

			void setFontColor(const sf::Color& color) noexcept
			{
				m_text.setFillColor(color);
			}

			void setFontSize(mlk::uint size) noexcept
			{
				m_text.setCharacterSize(size);
				this->calculate_textpos();
			}

			void setFillColor(const sf::Color& color) noexcept
			{
				m_shape.setFillColor(color);
				m_restore_shape.setFillColor(color);
			}

			void setOutlineThickness(float thickness) noexcept
			{
				m_shape.setOutlineThickness(thickness);
				m_restore_shape.setOutlineThickness(thickness);
			}

			void setOutlineColor(const sf::Color& color) noexcept
			{
				m_shape.setOutlineColor(color);
				m_restore_shape.setOutlineColor(color);
			}

			void setPosition(const vec2f& pos) noexcept
			{
				m_shape.setPosition(pos);
				m_restore_shape.setPosition(pos);
				this->calculate_textpos();
			}

			void setOrigin(const vec2f& pos) noexcept
			{
				m_shape.setOrigin(pos);
				m_restore_shape.setOrigin(pos);
				this->calculate_textpos();
			}

			void setSize(const vec2f& size) noexcept
			{
				m_shape.setSize(size);
				m_restore_shape.setSize(size);
				this->calculate_textpos();
			}

			void setTexture(sf::Texture* tx) noexcept
			{
				m_shape.setTexture(tx);
				m_restore_shape.setTexture(tx);
			}

			void move(const vec2f& offset) noexcept
			{
				m_shape.move(offset);
				m_restore_shape.move(offset);
				this->calculate_textpos();
			}

			void rotate(float angle) noexcept
			{
				m_shape.rotate(angle);
				m_restore_shape.rotate(angle);
				this->calculate_textpos();
			}

			std::string getText() const noexcept { return m_text.getString(); }

			const sf::Font* getFont() const noexcept
			{
				return m_text.getFont();
			}

			const sf::Color& getFontColor() const noexcept
			{
				return m_text.getFillColor();
			}

			mlk::uint getFontSize() const noexcept
			{
				return m_text.getCharacterSize();
			}

			const sf::Color& getFillColor() const noexcept
			{
				return m_shape.getFillColor();
			}

			float getOutlineThickness() const noexcept
			{
				return m_shape.getOutlineThickness();
			}

			const sf::Color& getOutlineColor() const noexcept
			{
				return m_shape.getOutlineColor();
			}

			const vec2f& getPosition() const noexcept
			{
				return m_shape.getPosition();
			}

			const vec2f& getOrigin() const noexcept
			{
				return m_shape.getOrigin();
			}

			const vec2f& getSize() const noexcept { return m_shape.getSize(); }

			const auto getGlobalBounds() const noexcept
			{
				return m_shape.getGlobalBounds();
			}

			const sf::Texture* getTexture() const noexcept
			{
				return m_shape.getTexture();
			}

			bool pressed() const noexcept { return m_press; }

			bool hover() const noexcept { return m_hover; }

		protected:
			virtual void init()
			{
				// init the rect
			}

			void init_base()
			{
				this->init();
				this->create_restore_shape();
			}

			virtual void on_hover()
			{
				// do something on hover
			}

			virtual void on_hover_end()
			{
				if(m_press) return;
				this->restore_origin();
			}

			virtual void on_press()
			{
				// do something on press
			}

			virtual void on_press_end()
			{
				if(m_hover) return;
				this->restore_origin();
			}

			void create_restore_shape() noexcept { m_restore_shape = m_shape; }

			void restore_origin() noexcept { m_shape = m_restore_shape; }

			virtual void draw(sf::RenderTarget& target,
							  sf::RenderStates states) const override
			{
				target.draw(m_shape, states);
				target.draw(m_text, states);
			}

		private:
			void calculate_textpos()
			{
				auto shape_bounds{m_shape.getGlobalBounds()};
				auto text_bounds{m_text.getGlobalBounds()};
				m_text.setOrigin(text_bounds.width / 2.f,
								 text_bounds.height / 2.f);
				m_text.setPosition(shape_bounds.left + shape_bounds.width / 2.f,
								   shape_bounds.top +
									   shape_bounds.height / 2.f);
			}
		};
	}// namespace ui
}// namespace rj

#endif// RJ_UI_BUTTON_HPP
