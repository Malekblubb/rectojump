//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_EDITOR_EDITOR_HPP
#define RJ_GAME_EDITOR_EDITOR_HPP


#include "background_editor.hpp"
#include "editor_entity.hpp"
#include "itembar.hpp"
#include "mouse.hpp"
#include "settingsbar.hpp"
#include <rectojump/game/components/platform.hpp>
#include <rectojump/game/camera.hpp>
#include <rectojump/game/popup_manager.hpp>
#include <rectojump/game/world.hpp>


namespace rj
{
	template<typename Game_Handler, typename Game>
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

		camera m_editarea_camera;
		camera m_itembar_camera;
		camera m_settingsbar_camera;

		editor_mouse<Game_Handler> m_mouse;
		background_editor<editor> m_background{*this};
		itembar<Game_Handler> m_itembar;
		settingsbar<editor> m_settingsbar;

	public:
		editor(Game_Handler& gh) :
			m_gamehandler{gh},
			m_game{gh.get_game()},
			m_gameworld{m_game.get_world()},
			m_entityhandler{m_gameworld.get_entityhandler()},
			m_levelmgr{gh.get_levelmgr()},
			m_editarea_camera{m_gamehandler.get_gamewindow()},
			m_itembar_camera{m_gamehandler.get_gamewindow()},
			m_settingsbar_camera{m_gamehandler.get_gamewindow()},
			m_mouse{gh},
			m_itembar{gh, {settings::get_window_size<vec2f>().x, 100.f}},
			m_settingsbar{*this, {300.f, settings::get_window_size<vec2f>().y - m_itembar.get_size().y}}
		{this->init();}

		void update(dur duration)
		{
			// edit area
			m_editarea_camera.set_changes();
			m_mouse.update(duration);

			// itembar
			m_itembar_camera.set_changes();
			m_itembar.update(duration);

			// settingsbar
			m_settingsbar_camera.set_changes();
			m_settingsbar.update(duration);
		}

		void update_input()
		{
			m_settingsbar.update_input();
		}

		void render()
		{
			// edit area
			m_editarea_camera.set_changes();
			m_entityhandler.render();
			m_mouse.render();

			// itembar
			m_itembar_camera.set_changes();
			m_itembar.render();

			// settingsbar
			m_settingsbar_camera.set_changes();
			m_settingsbar.render();
		}

		void on_activate()
		{
			m_background.reset();
		}

		void handle_save(const level_id& level_name)
		{
			if(!this->check_level_name(level_name))
				return;

			level_data lv_data;
			for(auto& a : m_entityhandler)
			{
				auto ent(this->to_editor_entity(a));
				lv_data.add_entity(ent->get_figure(), entity_propertie::solid, ent->pos());

			}

			level_info lv_info{level_name, "Creator", "Date"};
			music_data lv_music{'M', 'U', 'S', 'I', 'C'};
			level_packer<packer_mode::pack> lv_packer{lv_music, lv_data, lv_info};

			m_levelmgr.save_level(lv_packer, level_name);
		}

		void handle_load(const level_id& level_name)
		{
			if(!this->check_level_name(level_name))
				return;

//			auto& lv(m_levelmgr.get_level(level_name));
//			m_gameworld.load_level(lv.entities);
		}

		auto get_gamehandler() noexcept
		-> decltype(m_gamehandler)&
		{return m_gamehandler;}

	private:
		void init()
		{
			// change mousetexture on itembar-item click
			m_itembar.on_item_click =
			[this](ui::base_btn_ptr& b)
			{
				m_mouse.set_texture(b->get_texture());
				m_mouse.deactivate_selection();
				m_mouse.set_mouse_visible(true);
			};

			// init mouse texture
			m_mouse.set_texture(&m_itembar.get_current_texture());

			this->init_input();
			this->init_cameras();
		}

		void init_input()
		{
			m_gamehandler.template add_input<state::editor>(
			[this](const vec2f& pos){this->on_mouse_left(pos);}, btn::Left);

			m_gamehandler.template add_input<state::editor>(
			[this](const vec2f& pos){this->on_mouse_right(pos);}, btn::Right);

			m_gamehandler.template add_input<state::editor>(
			[this](const vec2f& pos)
			{m_editarea_camera.move({settings::get_editor_scroll_step(), 0.f});}, wheel::down);

			m_gamehandler.template add_input<state::editor>(
			[this](const vec2f& pos)
			{
				if(m_editarea_camera.get_center().x >= m_editarea_camera.get_startcenter().x)
					m_editarea_camera.move({-settings::get_editor_scroll_step(), 0.f});
			}, wheel::up);
		}

		void init_cameras() noexcept
		{
			auto window_size(settings::get_window_size<vec2f>());
			auto& itembar_size(m_itembar.get_size());
			auto& settingsbar_size(m_settingsbar.get_size());

			float itembar_top{(window_size.y - itembar_size.y) / window_size.y};
			float itembar_height{itembar_size.y / window_size.y};
			float editarea_height{(window_size.y - itembar_size.y) / window_size.y};


			sf::View editarea_view{vec2f{window_size.x, window_size.y - itembar_size.y} / 2.f, {window_size.x, window_size.y - itembar_size.y}};
			editarea_view.setViewport({0.f, 0.f, 1.f ,editarea_height});
			m_editarea_camera.set_view(editarea_view);

			sf::View itembar_view{vec2f{window_size.x, itembar_size.y} / 2.f, {window_size.x, itembar_size.y}};
			itembar_view.setViewport({0.f, itembar_top, 1.f, itembar_height});
			m_itembar_camera.set_view(itembar_view);

			sf::View settingsbar_view{vec2f{settingsbar_size.x, window_size.y} / 2.f, {settingsbar_size.x, window_size.y}};
			settingsbar_view.setViewport({(window_size.x - settingsbar_size.x) / window_size.x, 0.f, settingsbar_size.x / window_size.x, 1.f});
			m_settingsbar_camera.set_view(settingsbar_view);
		}

		void on_mouse_left(const vec2f&)
		{
			// check the itembar and settingsbar bounds
			auto itembar_mouse_bounds(bounds_from_vec(m_itembar_camera.get_mapped_mousepos()));
			auto settingsbar_mouse_bounds(bounds_from_vec(m_settingsbar_camera.get_mapped_mousepos()));
			if(itembar_mouse_bounds.intersects(m_itembar.get_bounds()) ||
			   settingsbar_mouse_bounds.intersects(m_settingsbar.get_bounds()))
				return;


			m_editarea_camera.set_changes();

			if(m_mouse.is_selection_visible())
			{
				if(m_mouse.get_selected().size() == 0)
					m_mouse.deactivate_selection();

				for(auto& a : m_mouse.get_selected())
				{
					auto new_ent(m_gameworld.template create_entity<editor_entity>(a.pos()));
					new_ent->set_texture(a.get_render_object().getTexture());
					new_ent->set_figure(a.get_figure());
				}
			}
			else if(m_mouse.is_mouse_visible())
			{
				float f{48.f};
				vec2f new_pos{round_to(m_editarea_camera.get_mapped_mousepos().x, f), round_to(m_editarea_camera.get_mapped_mousepos().y, f)};

				// set entity at pos
				if(m_mouse.get_texture())
				{
					auto ptr(m_gameworld.template create_entity<editor_entity>(new_pos));
					ptr->render_object().setTexture(m_mouse.get_texture());
					ptr->set_figure(m_itembar.get_current_figure());
				}
			}
			else
				m_mouse.selection_start();
		}

		void on_mouse_right(const vec2f&)
		{
			m_itembar.deselect_all();
			m_mouse.clear();

			auto iter(m_entityhandler.exists_entity_at(m_editarea_camera.get_mapped_mousepos()));
			if(iter != std::end(m_entityhandler))
				m_entityhandler.delete_entity(iter);
		}

		template<typename Ent_Ptr>
		auto to_editor_entity(const Ent_Ptr& ptr)
		-> decltype(std::static_pointer_cast<editor_entity>(ptr))
		{return std::static_pointer_cast<editor_entity>(ptr);}

		bool check_level_name(const level_id& id) const noexcept
		{
			if(!id.empty())
				return true;
			m_gamehandler.get_popupmgr().template create_popup<popup_type::error>("Couldn't save level: " + mlk::lerr_i().error_str(errors::lv_bad_name));
			return false;
		}
	};
}


#endif // RJ_GAME_EDITOR_EDITOR_HPP
