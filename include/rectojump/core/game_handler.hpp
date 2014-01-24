//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_CORE_GAME_HANDLER_HPP
#define RJ_CORE_GAME_HANDLER_HPP


#include "game.hpp"
#include "game_window.hpp"
#include "render.hpp"
#include <rectojump/game/background/background_manager.hpp>
#include <rectojump/game/editor/editor.hpp>
#include <rectojump/game/main_menu/main_menu.hpp>
#include <rectojump/game/popup_manager.hpp>
#include <rectojump/global/common.hpp>
#include <rectojump/global/config_settings.hpp>
#include <rectojump/shared/data_manager.hpp>
#include <rectojump/shared/level_manager/level_manager.hpp>

#include <mlk/containers/container_utl.h>
#include <mlk/tools/bitset.h>


namespace rj
{
	enum class state : std::size_t
	{main_menu, game_menu, game, editor, debug_info, num};

	class game_handler
	{
		game_window& m_game_window;
		data_manager& m_datamgr;
		level_manager& m_lvmgr;

		render<game_handler> m_render;
		background_manager m_backgroundmgr;
		game<game_handler> m_game;
		editor<game_handler> m_editor;
		main_menu<game_handler> m_mainmenu;
		popup_manager<game_handler> m_popupmgr;
		debug_info<game_handler, decltype(m_game)> m_debug_info;

		mlk::ebitset<state, state::num> m_current_states;

	public:
		game_handler(game_window& gw, data_manager& dm, level_manager& lm) :
			m_game_window{gw},
			m_datamgr{dm},
			m_lvmgr{lm},
			m_render{*this},
			m_backgroundmgr{m_render},
			m_game{*this},
			m_editor{*this},
			m_mainmenu{*this},
			m_popupmgr{*this},
			m_debug_info{*this}
		{this->init();}

		void load_level(const level_id& id)
		{
			auto& lv(m_lvmgr.get_level(id));
			if(!lv.is_valid())
			{
				m_popupmgr.create_popup<popup_type::error>("Failed to load level: not a valid level");
				return;
			}

			m_backgroundmgr.clear();

			this->deactivate_state(state::main_menu);
			this->activate_state(state::game);

			m_game.load_level(lv);
		}

		template<typename... Args>
		void render_object(Args&&... args)
		{m_game_window.draw(std::forward<Args>(args)...);}

		// getters
		auto get_render() noexcept
		-> decltype(m_render)&
		{return m_render;}

		auto get_gamewindow() noexcept
		-> decltype(m_game_window)&
		{return m_game_window;}

		auto get_game() noexcept
		-> decltype(m_game)&
		{return m_game;}

		auto get_backgroundmgr() noexcept
		-> decltype(m_backgroundmgr)&
		{return m_backgroundmgr;}

		auto get_mainmenu() noexcept
		-> decltype(m_mainmenu)&
		{return m_mainmenu;}

		auto get_datamgr() noexcept
		-> decltype(m_datamgr)&
		{return m_datamgr;}

		auto get_levelmgr() noexcept
		-> decltype(m_lvmgr)&
		{return m_lvmgr;}

		auto get_debuginfo() noexcept
		-> decltype(m_debug_info)&
		{return m_debug_info;}

		auto get_popupmgr() noexcept
		-> decltype(m_popupmgr)&
		{return m_popupmgr;}

	private:
		void init()
		{
			m_current_states |= state::main_menu;

			m_game_window.get_updater().on_update += [this](dur duration){this->update(duration);};
			m_game_window.get_updater().on_render += [this]{this->render();};

			settings::on_changed() +=
			[this]{m_game_window.set_size(settings::get_window_size());};

			m_mainmenu.get_menu_start()->get_items().on_event("editor",
			[this]
			{
				this->deactivate_state(state::main_menu);
				this->activate_state(state::editor);
				m_editor.on_activate();
			});

			this->init_pointers();
			this->init_input();
		}

		void init_pointers() noexcept
		{
			m_game.set_levelmgr(&m_lvmgr);
		}

		void init_input()
		{
			// global input
			on_key_pressed(key::F) +=
			[this]
			{
				m_game_window.toggle_fullscreen();
				rj::settings::set_fullscreen(m_game_window.get_fullscreen());
			};

			on_key_pressed(key::T) +=
			[this]{m_game_window.toggle_titlebar();};

			on_keys_pressed(key::LShift, key::D) +=
			[this]{this->toggle_state(state::debug_info);};

			on_keys_pressed(key::LShift, key::Q) +=
			[this]{m_game_window.stop();};

			// window input
			on_key_pressed(key::Escape) +=
			[this]{if(!this->is_active(state::main_menu)) return; m_game_window.stop();};

			// menu input
			on_key_pressed(key::Return) +=
			[this]
			{
				if(!this->is_active(state::main_menu))
					return;
				m_mainmenu.call_current_itemevent();
			};

			on_key_pressed(key::BackSpace) +=
			[this]
			{
				if(!this->is_active(state::main_menu))
					return;
				m_mainmenu.on_key_backspace();
			};

			on_key_pressed(key::Up) +=
			[this]
			{
				if(!this->is_active(state::main_menu))
					return;
				m_mainmenu.on_key_up();
			};

			on_keys_pressed(key::Down) +=
			[this]
			{
				if(!this->is_active(state::main_menu))
					return;
				m_mainmenu.on_key_down();
			};

			// game input
			on_key_pressed(key::P) +=
			[this]
			{
				if(this->is_active(state::game))
				{
					m_current_states.remove(state::game);
					m_current_states |= state::game_menu;
					return;
				}
				else if(this->is_active(state::game_menu))
				{
					m_current_states.remove(state::game_menu);
					m_current_states |= state::game;
					return;
				}
			};

			on_key_pressed(key::D) +=
			[this]{if(!this->is_active(state::game)) return; m_game.get_world().c_player();};

			on_btn_pressed(btn::Left) +=
			[this](const vec2f& pos)
			{
				if(!this->is_active(state::game))
					return;
				m_game.get_world().c_ent(pos);
			};

			on_btn_pressed(btn::Right) +=
			[this](const vec2f& pos)
			{
				if(!this->is_active(state::game))
					return;
				m_game.get_world().c_ent_death(pos);
			};

			on_key_pressed(key::A) +=
			[this]
			{
				if(!this->is_active(state::game))
					return;
				m_game.get_world().c_world();
			};
		}

		void activate_state(state s)
		{m_current_states |= s;}

		void deactivate_state(state s)
		{m_current_states.remove(s);}

		void toggle_state(state s)
		{m_current_states.toggle(s);}

		bool is_active(state s) const
		{return m_current_states & s;}

		void set_only_state(state s)
		{
			m_current_states.remove_all();
			m_current_states |= s;
		}

		void update(dur duration)
		{
			m_backgroundmgr.update(duration);

			if(this->is_active(state::game))
				m_game.update(duration);

			else if(this->is_active(state::editor))
				m_editor.update(duration);

			else if(this->is_active(state::main_menu))
				m_mainmenu.update(duration);

			if(this->is_active(state::debug_info))
				m_debug_info.update(duration);

			m_popupmgr.update(duration);
		}

		void render()
		{
			m_backgroundmgr.render();

			// render game when in game menu
			if(this->is_active(state::game) || this->is_active(state::game_menu))
				m_game.render();

			else if(this->is_active(state::editor))
				m_editor.render();

			else if(this->is_active(state::main_menu))
				m_mainmenu.render();

			if(this->is_active(state::debug_info))
				m_debug_info.render();

			m_popupmgr.render();
		}
	};
}



#endif // RJ_CORE_GAME_HANDLER_HPP
