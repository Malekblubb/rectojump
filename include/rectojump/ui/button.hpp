//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_UI_BUTTON_HPP
#define RJ_UI_BUTTON_HPP


#include <rectojump/global/common.hpp>
#include <rectojump/shared/input.hpp>

#include <mlk/types/types.h>


namespace rj
{
	namespace ui
	{
		class button : public sf::Drawable
		{
		protected:
			sf::RectangleShape m_shape;
			sf::Text m_text;
			bool m_hover{false};
			bool m_press{false};

		private:
			sf::RectangleShape m_restore_shape;

		public:
			button(const vec2f& size = {0.f, 0.f}, const vec2f& pos = {0.f, 0.f}) :
				m_shape{size}
			{
				m_shape.setPosition(pos);
				this->init_base();
			}

			button(const vec2f& size, const std::string& text, const sf::Font& font, const sf::Color& fontcolor = {}) :
				button{size}
			{
				m_text.setString(text);
				m_text.setFont(font);
				m_text.setColor(fontcolor);
				this->calculate_textpos();
			}

			void update(dur)
			{
				// check states
				if(m_hover) this->on_hover();
				else this->on_hover_end();

				if(m_press) this->on_press();
				else this->on_press_end();

				// collision
				auto bounds(m_shape.getGlobalBounds());
				auto mousebounds(get_mousebounds<true>());

				if(bounds.intersects(mousebounds))
					m_hover = true;
				else m_hover = false;

				if(m_hover && is_btn_pressed(btn::Left))
					m_press = true;
				else m_press = false;
			}

			void set_text(const std::string& text) noexcept
			{m_text.setString(text); this->calculate_textpos();}

			void set_font(const sf::Font& font) noexcept
			{m_text.setFont(font); this->calculate_textpos();}

			void set_fontcolor(const sf::Color& color) noexcept
			{m_text.setColor(color);}

			void set_fontsize(mlk::uint size) noexcept
			{m_text.setCharacterSize(size); this->calculate_textpos();}

			void set_color(const sf::Color& color) noexcept
			{m_shape.setFillColor(color); m_restore_shape.setFillColor(color);}

			void set_outlinethickness(float thickness) noexcept
			{m_shape.setOutlineThickness(thickness); m_restore_shape.setOutlineThickness(thickness);}

			void set_outlinecolor(const sf::Color& color) noexcept
			{m_shape.setOutlineColor(color); m_restore_shape.setOutlineColor(color);}

			void set_position(const vec2f& pos) noexcept
			{m_shape.setPosition(pos); m_restore_shape.setPosition(pos); this->calculate_textpos();}

			void set_origin(const vec2f& pos) noexcept
			{m_shape.setOrigin(pos); m_restore_shape.setOrigin(pos); this->calculate_textpos();}

			void set_size(const vec2f& size) noexcept
			{m_shape.setSize(size); m_restore_shape.setSize(size); this->calculate_textpos();}

			void set_texture(sf::Texture* tx) noexcept
			{m_shape.setTexture(tx); m_restore_shape.setTexture(tx);}

			void move(const vec2f& offset) noexcept
			{m_shape.move(offset); m_restore_shape.move(offset); this->calculate_textpos();}

			void rotate(float angle) noexcept
			{m_shape.rotate(angle); m_restore_shape.rotate(angle); this->calculate_textpos();}

			std::string get_text() const noexcept
			{return m_text.getString();}

			const sf::Font* get_font() const noexcept
			{return m_text.getFont();}

			const sf::Color& get_fontcolor() const noexcept
			{return m_text.getColor();}

			mlk::uint get_fontsize() const noexcept
			{return m_text.getCharacterSize();}

			const sf::Color& get_color() const noexcept
			{return m_shape.getFillColor();}

			float get_outlinethickness() const noexcept
			{return m_shape.getOutlineThickness();}

			const sf::Color& get_outlinecolor() const noexcept
			{return m_shape.getOutlineColor();}

			const vec2f& get_position() const noexcept
			{return m_shape.getPosition();}

			const vec2f& get_origin() const noexcept
			{return m_shape.getOrigin();}

			const vec2f& get_size() const noexcept
			{return m_shape.getSize();}

			const sf::Texture* get_texture() const noexcept
			{return m_shape.getTexture();}

			bool is_pressed() const noexcept
			{return m_press;}

			bool is_hover() const noexcept
			{return m_hover;}

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

			void create_restore_shape() noexcept
			{m_restore_shape = m_shape;}

			void restore_origin() noexcept
			{m_shape = m_restore_shape;}

			virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override
			{
				target.draw(m_shape, states);
				target.draw(m_text, states);
			}

		private:
			void calculate_textpos()
			{
				auto text_bounds(m_text.getGlobalBounds());
				m_text.setOrigin(text_bounds.width / 2.f, text_bounds.height / 2.f);
				auto shape_bounds(m_shape.getGlobalBounds());
				auto shape_size(m_shape.getSize());
				m_text.setPosition({shape_bounds.left + shape_size.x / 2.f, shape_bounds.top + shape_size.y / 2.f});
			}
		};
	}
}


#endif // RJ_UI_BUTTON_HPP
