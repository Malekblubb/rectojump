//
// Copyright (c) 2013-2017 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_UI_TEXTBOX_HPP
#define RJ_UI_TEXTBOX_HPP


#include "widget.hpp"
#include <rectojump/global/common.hpp>
#include <rectojump/shared/input.hpp>
#include <rectojump/shared/utils.hpp>

#include <mlk/containers/container_utl.h>
#include <mlk/time/simple_timer.h>

#include <stack>


namespace rj
{
	namespace ui
	{
		class textbox : public widget, public sf::Transformable
		{
			sf::RectangleShape m_shape;
			sf::Text m_text, m_password_text;
			sf::VertexArray m_cursor{sf::Lines};
			sf::Color m_cursorcolor{sf::Color::Black};
			std::stack<char> m_char_stack;

			mlk::ullong m_cursor_blinkinterval{700};
			mlk::tm::simple_timer m_blinktimer{m_cursor_blinkinterval};
			bool m_focus{false}, m_cursor_visible{true}, m_password_mode{false};

		public:
			mlk::slot<> on_key_return;

			textbox() = default;

			textbox(const vec2f& size, const vec2f& pos, const sf::Font& font, const std::string& text = "") :
				m_shape{size},
				m_text{text, font},
				m_password_text{"", font}
			{
				this->setPosition(pos);
				this->init();
			}

			void update(dur) override
			{
				if(inp::was_real_mousepress_left())
				{
					if(this->getGlobalBounds().intersects(inp::get_mousebounds<true>()))
						m_focus = true;
					else m_focus = false;
				}

				if(!m_focus)
					return;

				// input
				if(inp::get_last_textinput() != 0)
					this->update_text(inp::get_last_textinput());

				// cursor
				if(m_blinktimer.timed_out())
				{
					m_cursor_visible = !m_cursor_visible;
					m_blinktimer.restart();
				}
			}

			void addChar(char c) noexcept
			{if(!m_focus) return; this->update_text(c);}

			// sfml-func-style interface
			// setters
			void setSize(const vec2f& size) noexcept
			{m_shape.setSize(size); this->update_text_pos(); this->update_cursor();}

			void setFillColor(const sf::Color& color) noexcept
			{m_shape.setFillColor(color);}

			void setOutlineColor(const sf::Color& color) noexcept
			{m_shape.setOutlineColor(color);}

			void setOutlineThickness(float thickness) noexcept
			{m_shape.setOutlineThickness(thickness);}

			void setCursorBlinkInterval(mlk::ullong interval) noexcept
			{m_cursor_blinkinterval = interval;}

			void setCursorColor(const sf::Color& color) noexcept
			{m_cursorcolor = color; this->update_cursor();}

			void setTextColor(const sf::Color& color) noexcept
            {m_text.setFillColor(color); m_password_text.setFillColor(color);}

			void setTextSize(mlk::uint size) noexcept
			{m_text.setCharacterSize(size); m_password_text.setCharacterSize(size); this->update_text_pos(); this->update_cursor();}

			void setText(const std::string& str)
			{
				m_text.setString("");
				m_char_stack = std::stack<char>{};
				for(const auto& a : str)
					this->update_text(a);
			}

			void setPasswordMode(bool b) noexcept
			{m_password_mode = b; this->update_text_pos(); this->update_cursor();}

			// getters
			const vec2f& getSize() const noexcept
			{return m_shape.getSize();}

			const sf::Color& getFillColor() const noexcept
			{return m_shape.getFillColor();}

			const sf::Color& getOutlineColor() const noexcept
			{return m_shape.getOutlineColor();}

			float getOutlineThickness() const noexcept
			{return m_shape.getOutlineThickness();}

			mlk::ullong getCursorBlinkInterval() const noexcept
			{return m_cursor_blinkinterval;}

			const sf::Color& getCursorColor() const noexcept
			{return m_cursorcolor;}

			const sf::Color& getTextColor() const noexcept
            {return m_text.getFillColor();}

			mlk::uint getTextSize() const noexcept
			{return m_text.getCharacterSize();}

            std::string getText() const noexcept
			{
				std::string result;
				auto char_stack(m_char_stack);
				auto size(char_stack.size());
				for(std::size_t i{0}; i < size; ++i)
				{
					result += char_stack.top();
					char_stack.pop();
                }
                mlk::cnt::reverse_all(result);
                return result += m_text.getString();
            }

			sf::FloatRect getGlobalBounds() const noexcept
			{return {bounds_from_vec({this->getPosition().x - this->getOrigin().x, this->getPosition().y - this->getOrigin().y}, m_shape.getSize())};}

			bool getPasswordMode() const noexcept
			{return m_password_mode;}

		private:
			void init() noexcept
			{
				m_cursor.append({{0.f, 0.f}, m_cursorcolor});
				m_cursor.append({{0.f, 0.f}, m_cursorcolor});
				this->update_text_pos();
				this->update_cursor();
				this->update_password_mode();
				m_blinktimer.run();
			}

			void draw(sf::RenderTarget& target, sf::RenderStates states) const override
			{
				auto s(states);

				// shape
				s.transform = sf::Transformable::getTransform();
				target.draw(m_shape, s);

				// text
				if(m_password_mode)
					target.draw(m_password_text, s);
				else
					target.draw(m_text, s);

				// cursor
				if(m_cursor_visible && m_focus)
					target.draw(m_cursor, s);
			}

			void update_password_mode()
			{
				auto passwordchars(m_text.getString().toAnsiString());
				mlk::stl_string::set_all('*', passwordchars);
				m_password_text.setString(passwordchars);
			}

			void update_text(std::uint32_t u)
			{
				// return event
				if(u == 0x0d)
				{
					on_key_return();
					return;
				}

				if(u != 0x08 && (u < 0x20 || u > 0x7E))
					return;

				auto text(m_text.getString());
				auto shape_bounds(m_shape.getGlobalBounds());
				auto text_bounds(m_text.getGlobalBounds());

				// check bounds
				if(text_bounds.left + text_bounds.width > shape_bounds.left + shape_bounds.width)
				{
					// textbounds are bigger than shapebounds
					// save and delete first char
					m_char_stack.push(*text.begin());
					text.erase(0, 1);
				}

				// backspace
				if(u == 0x08)
				{
					// erase last char
					if(!text.isEmpty())
						text.erase(text.getSize() - 1, 1);

					// get last removed char
					if(!m_char_stack.empty())
					{
						text.insert(0, m_char_stack.top());
						m_char_stack.pop();
					}
				}
				else text += u; // append input char to full text

				m_text.setString(text);

				if(m_password_mode)
					this->update_password_mode();

				this->update_cursor();

                std::cout << text.toAnsiString() << std::endl;
			}

			void update_text_pos()
			{
				auto shape_bounds(m_shape.getGlobalBounds());
				auto text_bounds(m_text.getGlobalBounds());
				auto origin(vec2f{0.f, text_bounds.height} / 2.f);
				vec2f pos{shape_bounds.left, shape_bounds.top + shape_bounds.height / 2.f};

				m_text.setOrigin(origin);
				m_text.setPosition(pos);
				m_password_text.setOrigin(origin);
				m_password_text.setPosition(pos);
			}

			void update_cursor()
			{
				auto shape_bounds(m_shape.getGlobalBounds());
				auto text_bounds(m_password_mode ? m_password_text.getGlobalBounds() : m_text.getGlobalBounds());
				auto cursor_margin_tb(0.1f * shape_bounds.height); // top / bottom
				m_cursor[0].position = {shape_bounds.left + text_bounds.width, shape_bounds.top + cursor_margin_tb};
				m_cursor[0].color = m_cursorcolor;
				m_cursor[1].position = {shape_bounds.left + text_bounds.width, shape_bounds.top + shape_bounds.height - cursor_margin_tb};
				m_cursor[1].color = m_cursorcolor;
			}
		};
	}
}


#endif // RJ_UI_TEXTBOX_HPP
