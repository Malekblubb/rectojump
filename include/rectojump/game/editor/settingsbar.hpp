//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_EDITOR_SETTINGSBAR_HPP
#define RJ_GAME_EDITOR_SETTINGSBAR_HPP


#include "button_item.hpp"
#include <rectojump/global/common.hpp>
#include <rectojump/global/config_settings.hpp>
#include <rectojump/shared/level_manager/level_manager.hpp>
#include <rectojump/ui/connected_buttons.hpp>
#include <rectojump/ui/textbox.hpp>


namespace rj
{
	template<typename Editor>
	class settingsbar
	{
		Editor& m_editor;
		typename Editor::gh_type& m_gamehandler;
		rndr& m_render;

		// interface
		sf::RectangleShape m_shape;
		ui::connected_buttons m_buttons;
		ui::button m_toggle_bar_button{{16.f, 16.f}, {16.f, 16.f}};
		sf::Texture m_toggle_bar_button_tx;
		sf::Font m_font;
		std::vector<ui::textbox> m_textboxes;

		// moving
		bool m_is_expanded{true}, m_moving{false}, m_need_move_right{false}, m_need_move_left{false};
		float m_moved{0.f};
		float m_max_move{m_shape.getSize().x - 32.f};
		static constexpr float m_move_step{1.f};

	public:
		settingsbar(Editor& e, const vec2f& size) :
			m_editor{e},
			m_gamehandler{e.get_gamehandler()},
			m_render{m_gamehandler.get_render()},
			m_shape{size},
			m_toggle_bar_button_tx{m_gamehandler.get_datamgr().template get_as<sf::Texture>("arrow.png")},
			m_font{m_gamehandler.get_datamgr().template get_as<sf::Font>("Fipps-Regular.otf")}
		{this->init();}

		~settingsbar()
		{settings::set_editor_settings_expanded(m_is_expanded);}

		void update(dur duration)
		{
			for(auto& a : m_textboxes)
				a.update(duration);
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

		void update_input()
		{
			auto lasttextinput(get_last_textinput());
			for(auto& a : m_textboxes)
				if(lasttextinput)
					a.addChar(lasttextinput);
		}

		void render()
		{
			m_render(m_shape, m_toggle_bar_button, m_buttons, m_textboxes);
		}

		auto get_bounds() const noexcept
		-> decltype(m_shape.getGlobalBounds())
		{return m_shape.getGlobalBounds();}

		const vec2f& get_size() const noexcept
		{return m_shape.getSize();}

	private:
		void init()
		{
			// shape
			auto& shape_size(m_shape.getSize());
			m_shape.setOrigin(shape_size / 2.f);
			m_shape.setPosition(shape_size / 2.f);
			m_shape.setFillColor(settings::get_color_default_light());

			// toggle (expand) settings bar
			m_toggle_bar_button.set_origin(m_toggle_bar_button.get_size() / 2.f);
			m_toggle_bar_button.set_texture(&m_toggle_bar_button_tx);
			if(settings::get_editor_settings_expanded())
				flip_h(m_toggle_bar_button);
			else m_need_move_right = true;

			// buttons
			vec2f btn_size{80.f, 25.f};
			auto save_btn(m_buttons.add_button_event<button_item>(
			[this]{m_editor.handle_save(m_textboxes[0].get_text());}, btn_size, vec2f{shape_size.x / 2.f - 60.f, shape_size.y - btn_size.y}));
			this->prepare_button(*save_btn);
			save_btn->set_text("Save");

			auto load_btn(m_buttons.add_button_event<button_item>(
			[this]{m_editor.handle_load(m_textboxes[0].get_text());}, btn_size, vec2f{shape_size.x / 2.f + 60.f, shape_size.y - btn_size.y}));
			this->prepare_button(*load_btn);
			load_btn->set_text("Load");

			vec2f tb_size{200.f, 30.f};
			m_textboxes.emplace_back(tb_size, vec2f{shape_size.x / 2.f, shape_size.y - 60.f}, m_font, "Level Name");
			this->prepare_textbox(m_textboxes.back());

			m_textboxes.emplace_back(tb_size, vec2f{shape_size.x / 2.f, 60.f}, m_font, "BG begin color");
			this->prepare_textbox(m_textboxes.back());
			m_textboxes.emplace_back(tb_size, vec2f{shape_size.x / 2.f, 120.f}, m_font, "BG end color");
			this->prepare_textbox(m_textboxes.back());
		}

		template<typename Btn_Type>
		void prepare_button(Btn_Type& btn)
		{
			btn.set_origin(btn.get_size() / 2.f);
			btn.set_font(m_font);
			btn.set_fontsize(12);
			btn.set_fontcolor(settings::get_color_light());
		}

		void prepare_textbox(ui::textbox& tb)
		{
			tb.setOrigin(tb.getSize() / 2.f);
			tb.setTextColor(settings::get_color_light());
			tb.setTextSize(12);
			tb.setOutlineThickness(2.f);
			tb.setOutlineColor(settings::get_color_default_dark());
			tb.setFillColor({0, 0, 0, 0});
		}

		void move(const vec2f& offset) noexcept
		{
			m_shape.move(offset);
			m_toggle_bar_button.move(offset);
			for(auto& a : m_buttons.get_buttons())
				a.second.button->move(offset);
			for(auto& a : m_textboxes)
				a.move(offset);
		}
	};
}


#endif // RJ_GAME_EDITOR_SETTINGSBAR_HPP
