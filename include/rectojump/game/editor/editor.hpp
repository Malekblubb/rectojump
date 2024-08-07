//
// Copyright (c) 2013-2021 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_EDITOR_EDITOR_HPP
#define RJ_GAME_EDITOR_EDITOR_HPP

#include "background_editor.hpp"
#include "editor_entity.hpp"
#include "itembar.hpp"
#include "mouse.hpp"
#include "settingsbar.hpp"
#include <rectojump/game/camera.hpp>
#include <rectojump/game/components/platform.hpp>
#include <rectojump/game/popup_manager.hpp>
#include <rectojump/game/world.hpp>

namespace rj
{
	template <typename Game_Handler, typename Game>
	class editor
	{
	public:
		using gh_type = Game_Handler;

	private:
		Game_Handler& m_gamehandler;
		Game& m_game;
		world<Game_Handler>& m_gameworld;
		entity_handler& m_entityhandler;
		level_manager& m_levelmgr;

		sf::RectangleShape m_camera_indicator_shape;
		camera m_editarea_camera;
		camera m_itembar_camera;
		camera m_settingsbar_camera;

		editor_mouse<Game_Handler> m_mouse;
		background_editor<editor> m_background{*this};
		itembar<Game_Handler> m_itembar;
		settingsbar<editor> m_settingsbar;

		bool m_is_level_loaded{false};
		level_id m_current_loaded_id{level_name_null};
		mlk::data_packet* m_musicdata{nullptr};

	public:
		editor(Game_Handler& gh)
			: m_gamehandler{gh},
			  m_game{gh.get_game()},
			  m_gameworld{m_game.get_world()},
			  m_entityhandler{m_gameworld.entityhandler()},
			  m_levelmgr{gh.levelmgr()},
			  m_editarea_camera{m_gamehandler.gamewindow()},
			  m_itembar_camera{m_gamehandler.gamewindow()},
			  m_settingsbar_camera{m_gamehandler.gamewindow()},
			  m_mouse{gh},
			  m_itembar{gh, {settings::get_window_size<vec2f>().x, 100.f}},
			  m_settingsbar{*this,
							{300.f, settings::get_window_size<vec2f>().y -
										m_itembar.get_size().y}}
		{
			this->init();
		}

		void update(dur duration)
		{
			// editareacamera move
			if(inp::is_btn_pressed(btn::Middle))
				m_editarea_camera.move(
					((inp::get_lastmousepos() - inp::get_mousepos()) / 6.f) *
					m_editarea_camera.get_zoomfactor() * duration);

			// edit area
			m_editarea_camera.activate();
			m_mouse.update(duration);

			// itembar
			m_itembar_camera.activate();
			m_itembar.update(duration);

			// settingsbar
			m_settingsbar_camera.activate();
			m_settingsbar.update(duration);
		}

		void update_input() { m_settingsbar.update_input(); }

		void render()
		{
			// edit area
			m_editarea_camera.activate();
			m_gamehandler.rendermgr()(m_camera_indicator_shape);
			m_entityhandler.render();
			m_mouse.render();

			// itembar
			m_itembar_camera.activate();
			m_itembar.render();

			// settingsbar
			m_settingsbar_camera.activate();
			m_settingsbar.render();
		}

		void on_activate()
		{
			//			m_background.reset(); TODO
		}

		bool load_musicfile(const std::string& path)
		{
			m_musicdata = m_gamehandler.datamgr().load_raw(path);
			return m_musicdata != nullptr;
		}

		void handle_save(const level_id& level_name)
		{
			if(!this->check_level_name(level_name)) return;

			// level background
			level_background lv_bg{m_settingsbar.get_tb_startcolor_text(),
								   m_settingsbar.get_tb_endcolor_text(),
								   mlk::stl_string::to_int<std::size_t>(
									   m_settingsbar.get_tb_pointcount_text())};

			// level data
			level_data lv_data;
			for(auto& a : m_entityhandler)
			{
				auto ent{this->to_editor_entity(a)};
				lv_data.add_entity(ent->get_figure(), entity_propertie::solid,
								   ent->pos());
			}

			level_info lv_info{level_name,
							   m_settingsbar.get_tb_lvcreator_text(),
							   mlk::tm::time_str()};
			music_data lv_music{header_music};
			if(m_musicdata != nullptr) mlk::cnt::append(*m_musicdata, lv_music);
			level_packer<packer_mode::pack> lv_packer{lv_music, lv_bg, lv_data,
													  lv_info};

			if(m_levelmgr.save_level(lv_packer, level_name))
			{
				mlk::lout("rj::editor")
					<< "Level \"" << level_name << "\" successfully saved!";
				m_gamehandler.popupmgr().template create_popup(
					"Successfully saved!");
			}
			else
			{
				mlk::lerr(errors::io_create_file)
					<< "(levelname=" << level_name << ")";
				m_gamehandler.popupmgr()
					.template create_popup<popup_type::error>("Save failed!");
			}
		}

		void handle_load(const level_id& level_name)
		{
			if(!this->check_level_name(level_name)) return;

			auto& lv{m_levelmgr.get_level(level_name)};
			m_gameworld.template load_level<true>(lv.entities);

			// set background
			auto& bg_shape{
				m_background.backgroundmgr().bg_shape(state::editor)};
			bg_shape.set_startcolor(lv.background.startcolor());
			bg_shape.set_endcolor(lv.background.endcolor());
			bg_shape.set_gradient_points(lv.background.pointcount());

			m_is_level_loaded = true;
			m_current_loaded_id = level_name;

			m_settingsbar.on_load(lv);
		}

		void handle_test()
		{
			if(!m_is_level_loaded) return;

			m_gamehandler.load_level(m_current_loaded_id);
		}

		void reset_zoom() noexcept { m_editarea_camera.reset_zoom(); }
		void reset_center() noexcept { m_editarea_camera.reset_center(); }
		auto& gamehandler() noexcept { return m_gamehandler; }
		camera& editarea_camera() noexcept { return m_editarea_camera; }
		camera& itembar_camera() noexcept { return m_itembar_camera; }
		camera& settingsbar_camera() noexcept { return m_settingsbar_camera; }

	private:
		void init()
		{
			// change mousetexture on itembar-item click
			m_itembar.on_item_click = [this](ui::base_btn_ptr& b) {
				m_mouse.set_texture(b->getTexture());
				m_mouse.deactivate_selection();
				m_mouse.set_mouse_visible(true);
			};

			// init mouse texture
			m_itembar.deselect_all();

			this->init_input();
			this->init_cameras();
		}

		void init_input()
		{
			m_gamehandler.template add_input<state::editor>(
				[this](const vec2f& pos) { this->on_mouse_left(pos); },
				btn::Left);

			m_gamehandler.template add_input<state::editor>(
				[this](const vec2f& pos) { this->on_mouse_right(pos); },
				btn::Right);

			m_gamehandler.template add_input<state::editor>(
				[this](const vec2f&) {
                    if(!inp::is_key_pressed(key::LControl))
						m_editarea_camera.move(
							{settings::get_editor_scroll_step(), 0.f});
				},
				wheel::down);

			m_gamehandler.template add_input<state::editor>(
				[this](const vec2f&) {
                    if(!inp::is_key_pressed(key::LControl))
						if(m_editarea_camera.get_center().x >=
						   m_editarea_camera.get_startcenter().x)
							m_editarea_camera.move(
								{-settings::get_editor_scroll_step(), 0.f});
				},
				wheel::up);

			m_gamehandler.template add_input<state::editor>(
				[this](const vec2f&) {
                    if(inp::is_key_pressed(key::LControl))
                        m_editarea_camera.zoom(0.9);
				},
				wheel::up);

			m_gamehandler.template add_input<state::editor>(
				[this](const vec2f&) {
                    if(inp::is_key_pressed(key::LControl))
                        m_editarea_camera.zoom(1.1);
				},
				wheel::down);
		}

		void init_cameras() noexcept
		{
			auto window_size{settings::get_window_size<vec2f>()};
			auto& itembar_size{m_itembar.get_size()};
			auto& settingsbar_size{m_settingsbar.get_size()};

			auto itembar_top{(window_size.y - itembar_size.y) / window_size.y};
			auto itembar_height{itembar_size.y / window_size.y};
			auto editarea_height{(window_size.y - itembar_size.y) /
								 window_size.y};

			sf::View editarea_view{
				vec2f{window_size.x, window_size.y - itembar_size.y} / 2.f,
				{window_size.x, window_size.y - itembar_size.y}};
			editarea_view.setViewport({0.f, 0.f, 1.f, editarea_height});
			m_editarea_camera.set_view(editarea_view);

			sf::View itembar_view{vec2f{window_size.x, itembar_size.y} / 2.f,
								  {window_size.x, itembar_size.y}};
			itembar_view.setViewport({0.f, itembar_top, 1.f, itembar_height});
			m_itembar_camera.set_view(itembar_view);

			sf::View settingsbar_view{vec2f{settingsbar_size.x, window_size.y} /
										  2.f,
									  {settingsbar_size.x, window_size.y}};
			settingsbar_view.setViewport(
				{(window_size.x - settingsbar_size.x) / window_size.x, 0.f,
				 settingsbar_size.x / window_size.x, 1.f});
			m_settingsbar_camera.set_view(settingsbar_view);
		}

		void on_mouse_left(const vec2f&)
		{
			// check the itembar and settingsbar bounds
			auto itembar_mouse_bounds{
				bounds_from_vec(m_itembar_camera.get_mapped_mousepos())};
			auto settingsbar_mouse_bounds{
				bounds_from_vec(m_settingsbar_camera.get_mapped_mousepos())};
			if(itembar_mouse_bounds.intersects(m_itembar.get_bounds()) ||
			   settingsbar_mouse_bounds.intersects(m_settingsbar.get_bounds()))
				return;

			m_editarea_camera.activate();

			if(m_mouse.is_selection_visible())
			{
				auto& selected{m_mouse.get_selected()};
				if(selected.size() == 0)
				{
					auto bounds{m_mouse.get_selectionshape_bounds()};
					this->delete_editor_entity({bounds.left, bounds.top},
											   {bounds.width, bounds.height});
				}

				for(auto& a : selected)
					if(!m_gameworld.entityhandler().exists_entity_at(a.pos()))
						this->create_editor_entity(a.pos(), a.get_texture(),
												   a.get_figure());
			}
			else if(m_mouse.is_mouse_visible())
			{
				auto f{48.f};
				vec2f new_pos{
					round_to(m_editarea_camera.get_mapped_mousepos().x, f),
					round_to(m_editarea_camera.get_mapped_mousepos().y, f)};

				// set entity at pos
				if(m_mouse.get_texture() &&
				   !m_gameworld.entityhandler().exists_entity_at(new_pos))
					this->create_editor_entity(new_pos, m_mouse.get_texture(),
											   m_itembar.get_current_figure());
			}
			else
				m_mouse.selection_start();
		}

		void on_mouse_right(const vec2f&)
		{
			m_itembar.deselect_all();
			m_mouse.clear();
			this->delete_editor_entity(m_editarea_camera.get_mapped_mousepos());
		}

		auto create_editor_entity(const vec2f& pos, const sf::Texture* tx,
								  entity_figure f)
		{
			auto ptr{m_gameworld.template create_entity<editor_entity>(pos)};
			ptr->set_texture(tx);
			ptr->set_figure(f);
			return ptr;
		}

		void delete_editor_entity(const vec2f& pos,
								  const vec2f& size = {1.f, 1.f})
		{
			auto iters{m_entityhandler.get_entities_at(pos, size)};
			m_entityhandler.delete_entities(iters);
		}

		template <typename Ent_Ptr>
		auto to_editor_entity(const Ent_Ptr& ptr)
		{
			return std::static_pointer_cast<editor_entity>(ptr);
		}

		bool check_level_name(const level_id& id) const noexcept
		{
			if(!id.empty()) return true;
			m_gamehandler.popupmgr().template create_popup<popup_type::error>(
				"Couldn't save level: " +
				mlk::lerr_i().error_str(errors::lv_bad_name));
			return false;
		}
	};
}// namespace rj

#endif// RJ_GAME_EDITOR_EDITOR_HPP
