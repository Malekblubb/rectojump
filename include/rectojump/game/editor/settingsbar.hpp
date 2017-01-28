//
// Copyright (c) 2013-2017 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_EDITOR_SETTINGSBAR_HPP
#define RJ_GAME_EDITOR_SETTINGSBAR_HPP


#include "button_item.hpp"
#include <rectojump/game/background/background_manager.hpp>
#include <rectojump/game/components/gradient_rect.hpp>
#include <rectojump/game/popup_manager.hpp>
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
		background_manager<typename Editor::gh_type>& m_backgroundmgr;
		rndr& m_render;

		// interface
		sf::RectangleShape m_shape;
		ui::connected_buttons m_buttons;
		ui::button m_toggle_bar_button{{16.f, 16.f}, {16.f, 16.f}};
		sf::Texture m_toggle_bar_button_tx;
		sf::Font m_font;
		std::map<std::string, ui::textbox> m_textboxes;

		// moving
		bool m_is_expanded{true}, m_moving{false}, m_need_move_right{false},
		m_need_move_left{false};
		float m_moved{0.f};
		float m_max_move{m_shape.getSize().x - 32.f};
		static constexpr float m_move_step{1.f};

	public:
		settingsbar(Editor& e, const vec2f& size) :
			m_editor{e},
			m_gamehandler{e.gamehandler()},
			m_backgroundmgr{m_gamehandler.backgroundmgr()},
			m_render{m_gamehandler.rendermgr()},
			m_shape{size},
			m_toggle_bar_button_tx{m_gamehandler.datamgr().template get_as<sf::Texture>
								   ("arrow.png")},
			m_font{m_gamehandler.datamgr().template get_as<sf::Font>("Fipps-Regular.otf")}
		{this->init();}

		~settingsbar()
		{settings::set_editor_settings_expanded(m_is_expanded);}

		void update(dur duration)
		{
			for(auto& a : m_textboxes)
				a.second.update(duration);
			m_buttons.update(duration);
			m_toggle_bar_button.update(duration);

			if(m_toggle_bar_button.pressed() && !m_moving)
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
		}

		void render()
		{
			m_render(m_shape, m_toggle_bar_button, m_buttons, m_textboxes);
		}

		std::string get_tb_startcolor_text() noexcept
		{return m_textboxes["tb_bgstartcolor"].getText();}

		std::string get_tb_endcolor_text() noexcept
		{return m_textboxes["tb_bgendcolor"].getText();}

		std::string get_tb_pointcount_text() noexcept
		{return m_textboxes["tb_bgpointcount"].getText();}

		std::string get_tb_lvcreator_text() noexcept
		{return m_textboxes["tb_lvinf_creator"].getText();}

		auto get_bounds() const noexcept
		-> decltype(m_shape.getGlobalBounds())
		{return m_shape.getGlobalBounds();}

		const vec2f& get_size() const noexcept
		{return m_shape.getSize();}

	private:
		void init()
		{
			// TODO: avoid magic numbers here

			// shape
			auto& shape_size(m_shape.getSize());
			m_shape.setOrigin(shape_size / 2.f);
			m_shape.setPosition(shape_size / 2.f);
			m_shape.setFillColor(settings::get_color_default_light());

			// toggle (expand) settings bar
			m_toggle_bar_button.setOrigin(m_toggle_bar_button.getSize() / 2.f);
			m_toggle_bar_button.setTexture(&m_toggle_bar_button_tx);
			if(settings::get_editor_settings_expanded())
				flip_h(m_toggle_bar_button);
			else m_need_move_right = true;

			// button size
			vec2f btn_size{80.f, 25.f};
			// textbox size
			vec2f tb_size{200.f, 30.f};

			// buttons
			auto reset_zoom_btn(m_buttons.add_button_event<button_item>(
									[this]
			{
				m_editor.reset_zoom();
			}, vec2f{160.f, 25.f}, vec2f{shape_size.x / 2.f, shape_size.y / 2.f}));
			this->prepare_button(*reset_zoom_btn);
			reset_zoom_btn->setText("Reset zoom");

			auto reset_center_btn(m_buttons.add_button_event<button_item>(
									  [this]
			{
				m_editor.reset_center();
			}, vec2f{160.f, 25.f}, vec2f{shape_size.x / 2.f, shape_size.y / 2.f + 30.f}));
			this->prepare_button(*reset_center_btn);
			reset_center_btn->setText("Reset center");

			// textboxes
			// level name
			m_textboxes.emplace("tb_lvname", ui::textbox{tb_size,
														 vec2f{shape_size.x / 2.f,
															   shape_size.y - 100.f},
														 m_font, "Level Name"});
			this->prepare_textbox(m_textboxes["tb_lvname"]);

			// level info
			m_textboxes.emplace("tb_lvinf_creator", ui::textbox{tb_size,
																vec2f{shape_size.x / 2.f,
																	  shape_size.y - 60.f},
																m_font, "Level Creator"});
			this->prepare_textbox(m_textboxes["tb_lvinf_creator"]);

			// backgroundcolors
			auto spacing(10.f);
			auto bg_return_key_func(
						[this]
			{
				auto start_color(to_rgb(m_textboxes["tb_bgstartcolor"].getText()));
				auto end_color(to_rgb(m_textboxes["tb_bgendcolor"].getText()));
				auto point_count(m_textboxes["tb_bgpointcount"].getText());
				if(!mlk::stl_string::is_numeric(point_count))
				{
					m_gamehandler.popupmgr().template create_popup<popup_type::error>
							("invalid content (must be numeric): " + point_count);
					return;
				}
				m_backgroundmgr.bg_shape(state::editor).set_startcolor(start_color);
				m_backgroundmgr.bg_shape(state::editor).set_endcolor(end_color);
				m_backgroundmgr.bg_shape(state::editor).set_gradient_points(
							mlk::stl_string::to_int<std::size_t>(point_count));
			});

			m_textboxes.emplace("tb_bgstartcolor", ui::textbox{tb_size,
															   vec2f{shape_size.x / 2.f,
																	 tb_size.y + spacing},
															   m_font, "BG begin color"});
			m_textboxes["tb_bgstartcolor"].on_key_return = bg_return_key_func;
			this->prepare_textbox(m_textboxes["tb_bgstartcolor"]);

			m_textboxes.emplace("tb_bgendcolor", ui::textbox{tb_size,
															 vec2f{shape_size.x / 2.f,
																   (tb_size.y + spacing) * 2},
															 m_font, "BG end color"});
			m_textboxes["tb_bgendcolor"].on_key_return = bg_return_key_func;
			this->prepare_textbox(m_textboxes["tb_bgendcolor"]);

			m_textboxes.emplace("tb_bgpointcount", ui::textbox{tb_size,
															   vec2f{shape_size.x / 2.f,
																	 (tb_size.y + spacing) * 3},
															   m_font, "BG gradient count"});
			m_textboxes["tb_bgpointcount"].on_key_return = bg_return_key_func;
			this->prepare_textbox(m_textboxes["tb_bgpointcount"]);

			// buttons
			auto btn_gradient_apply(m_buttons.add_button_event<button_item>
									(bg_return_key_func, btn_size,
									 vec2f{shape_size.x / 2.f, (tb_size.y + spacing) * 4}));
			btn_gradient_apply->setText("Apply");
			this->prepare_button(*btn_gradient_apply);

			auto go_back_btn(m_buttons.add_button_event<button_item>(
			[this]
			{
				m_gamehandler.switch_to_main_menu();
			}, vec2f{200.f, btn_size.y},
			vec2f{shape_size.x / 2.f,m_textboxes["tb_lvname"].getGlobalBounds().top - 30.f}));
			this->prepare_button(*go_back_btn);
			go_back_btn->setText("Back to Main Menu");

			auto save_btn(m_buttons.add_button_event<button_item>(
			[this]
			{
				m_editor.handle_save(m_textboxes["tb_lvname"].getText());
			}, btn_size, vec2f{shape_size.x / 2.f - 60.f, shape_size.y - btn_size.y}));
			this->prepare_button(*save_btn);
			save_btn->setText("Save");

			auto load_btn(m_buttons.add_button_event<button_item>(
							  [this]
			{
				m_editor.handle_load(m_textboxes["tb_lvname"].getText());
			}, btn_size, vec2f{shape_size.x / 2.f + 60.f, shape_size.y - btn_size.y}));
			this->prepare_button(*load_btn);
			load_btn->setText("Load");
		}

		template<typename Btn_Type>
		void prepare_button(Btn_Type& btn)
		{
			btn.setOrigin(btn.getSize() / 2.f);
			btn.setFont(m_font);
			btn.setFontSize(12);
			btn.setFontColor(settings::get_color_light());
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
				a.second.move(offset);
		}
	};
}


#endif // RJ_GAME_EDITOR_SETTINGSBAR_HPP
