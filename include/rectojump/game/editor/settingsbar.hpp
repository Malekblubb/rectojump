//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_EDITOR_SETTINGSBAR_HPP
#define RJ_GAME_EDITOR_SETTINGSBAR_HPP


#include "button_item.hpp"
#include <rectojump/global/common.hpp>
#include <rectojump/global/config_settings.hpp>
#include <rectojump/ui/connected_buttons.hpp>


namespace rj
{
	template<typename Game_Handler>
	class settingsbar
	{
		Game_Handler& m_gamehandler;
		rndr& m_render;

		// interface
		sf::RectangleShape m_shape;
		ui::connected_buttons m_buttons;
		ui::button m_toggle_bar_button{{16.f, 16.f}, {16.f, 16.f}};
		sf::Texture m_toggle_bar_button_tx;
		sf::Font m_font;

		// moving
		bool m_is_expanded{true}, m_moving{false}, m_need_move_right{false}, m_need_move_left{false};
		float m_moved{0.f};
		float m_max_move{m_shape.getSize().x - 32.f};
		static constexpr float m_move_step{1.f};

	public:
		settingsbar(Game_Handler& gh, const vec2f& size) :
			m_gamehandler{gh},
			m_render{gh.get_render()},
			m_shape{size},
			m_toggle_bar_button_tx{gh.get_datamgr().template get_as<sf::Texture>("arrow.png")},
			m_font{gh.get_datamgr().template get_as<sf::Font>("Fipps-Regular.otf")}
		{this->init();}

		void update(dur duration)
		{
			m_buttons.update(duration);
			m_toggle_bar_button.update(duration);

			if(m_toggle_bar_button.is_pressed() && !m_moving)
			{
				m_moved = 0.f;
				flip_h(m_toggle_bar_button);
				if(m_is_expanded) m_need_move_right = true;
				else m_need_move_left = true;
			}

			if(m_need_move_right)
			{
				if(m_moved > m_max_move)
				{
					m_moving = false;
					m_need_move_right = false;
					m_is_expanded = false;
					return;
				}

				m_moving = true;
				auto move_offset(vec2f{m_move_step, 0.f} * duration);
				this->move(move_offset);
				m_moved += move_offset.x;
			}


			if(m_need_move_left)
			{
				if(m_moved > m_max_move)
				{
					m_moving = false;
					m_need_move_left = false;
					m_is_expanded = true;
					return;
				}

				m_moving = true;
				auto move_offset(vec2f{-m_move_step, 0.f} * duration);
				this->move(move_offset);
				m_moved += -move_offset.x;
			}
		}

		void render()
		{
			m_render(m_shape, m_toggle_bar_button, m_buttons);
		}

		const vec2f& get_size() const noexcept
		{return m_shape.getSize();}

	private:
		void init()
		{
			auto& shape_size(m_shape.getSize());
			m_shape.setOrigin(shape_size / 2.f);
			m_shape.setPosition(shape_size / 2.f);
			m_shape.setFillColor(settings::get_color_default_light());

			m_toggle_bar_button.set_origin(m_toggle_bar_button.get_size() / 2.f);
			m_toggle_bar_button.set_texture(&m_toggle_bar_button_tx);
			flip_h(m_toggle_bar_button);

			vec2f btn_size{80.f, 40.f};
			auto save_btn(m_buttons.add_button_event<button_item>([]{}, btn_size, vec2f{shape_size.x / 2.f - 60.f, shape_size.y - btn_size.y}));
			save_btn->set_origin(btn_size / 2.f);
			save_btn->set_font(m_font);
			save_btn->set_fontsize(16);
			save_btn->set_text("Save");
			save_btn->set_fontcolor(settings::get_color_light());

			auto load_btn(m_buttons.add_button_event<button_item>([]{}, btn_size, vec2f{shape_size.x / 2.f + 60.f, shape_size.y - btn_size.y}));
			load_btn->set_origin(btn_size / 2.f);
			load_btn->set_font(m_font);
			load_btn->set_fontsize(16);
			load_btn->set_text("Load");
			load_btn->set_fontcolor(settings::get_color_light());
		}

		void move(const vec2f& offset)
		{
			m_shape.move(offset);
			m_toggle_bar_button.move(offset);
			for(auto& a : m_buttons.get_buttons())
				a.second.button->move(offset);
		}
	};
}


#endif // RJ_GAME_EDITOR_SETTINGSBAR_HPP
