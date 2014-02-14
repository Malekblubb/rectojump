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
#include <rectojump/game/camera.hpp>
#include <rectojump/game/editor/editor.hpp>
#include <rectojump/game/game_menu/game_menu.hpp>
#include <rectojump/game/main_menu/main_menu.hpp>
#include <rectojump/game/popup_manager.hpp>
#include <rectojump/global/common.hpp>
#include <rectojump/global/config_settings.hpp>
#include <rectojump/shared/level_manager/level_manager.hpp>
#include <rectojump/shared/data_manager.hpp>
#include <rectojump/shared/data_store.hpp>
#include <rectojump/shared/error_handler.hpp>

#include <mlk/containers/container_utl.h>
#include <mlk/tools/bitset.h>


namespace rj
{
	class game_handler
	{
	public:
		// type decls
		using data_store_type = data_store<sf::Texture, sf::Font>;

	private:
		game_window& m_game_window;
		error_handler& m_errorhandler;
		data_manager& m_datamgr;
		level_manager& m_lvmgr;

		data_store_type m_datastore{m_datamgr.get_all_containing_as_map_as<sf::Texture>(".png"), m_datamgr.get_all_containing_as_map_as<sf::Font>(".otf")};
		camera m_default_camera;
		render<game_handler> m_render;
		background_manager m_backgroundmgr;
		game<game_handler> m_game;
		editor<game_handler, decltype(m_game)> m_editor;
		main_menu<game_handler> m_mainmenu;
		game_menu<game_handler> m_gamemenu;
		popup_manager<game_handler> m_popupmgr;
		debug_info<game_handler, decltype(m_game)> m_debug_info;

		mlk::ebitset<state, state::num> m_current_states;

	public:
		game_handler(game_window& gw, error_handler& eh, data_manager& dm, level_manager& lm) :
			m_game_window{gw},
			m_errorhandler{eh},
			m_datamgr{dm},
			m_lvmgr{lm},
			m_default_camera{gw, {settings::get_window_size<vec2f>() / 2.f, settings::get_window_size<vec2f>()}},
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

			// (de)activate right states
			this->deactivate_state(state::main_menu);
			this->activate_state(state::game);

			// load background
			m_backgroundmgr.clear();
			auto& lv_bg(lv.background);
			auto& bgshape(m_backgroundmgr.bg_shape());
			bgshape.set_startcolor(lv_bg.get_startcolor());
			bgshape.set_endcolor(lv_bg.get_endcolor());
			bgshape.set_gradient_points(lv_bg.get_pointcount());

			// load level to gameworld
			m_game.load_level(lv);
		}

		template<typename... Args>
		void render_object(Args&&... args)
		{m_game_window.draw(std::forward<Args>(args)...);}

		template<state active_state, typename On_Input, typename... Input_Type_Args>
		void add_input(On_Input&& func, Input_Type_Args&&... keys_btns)
		{
			static_assert(sizeof...(keys_btns) > 0, "at least one key/button/wheel argument needed");
			add_input_helper<(sizeof...(keys_btns) > 1), typename std::tuple_element<0, std::tuple<Input_Type_Args...>>::type, active_state, On_Input, Input_Type_Args...>
			{*this, std::forward<On_Input>(func), std::forward<Input_Type_Args>(keys_btns)...};
		}

		// getters
		auto rendermgr() noexcept
		-> decltype(m_render)&
		{return m_render;}

		auto gamewindow() noexcept
		-> decltype(m_game_window)&
		{return m_game_window;}

		auto game() noexcept
		-> decltype(m_game)&
		{return m_game;}

		auto backgroundmgr() noexcept
		-> decltype(m_backgroundmgr)&
		{return m_backgroundmgr;}

		auto mainmenu() noexcept
		-> decltype(m_mainmenu)&
		{return m_mainmenu;}

		auto gamemenu() noexcept
		-> decltype(m_gamemenu)&
		{return m_gamemenu;}

		auto datastore() noexcept
		-> decltype(m_datastore)&
		{return m_datastore;}

		auto datamgr() noexcept
		-> decltype(m_datamgr)&
		{return m_datamgr;}

		auto levelmgr() noexcept
		-> decltype(m_lvmgr)&
		{return m_lvmgr;}

		auto debuginfo() noexcept
		-> decltype(m_debug_info)&
		{return m_debug_info;}

		auto popupmgr() noexcept
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

			m_mainmenu.mmenu_start()->get_items().on_event("editor",
			[this]
			{
				this->deactivate_state(state::main_menu);
				this->activate_state(state::editor);
				m_editor.on_activate();
			});

			this->init_errors();
			this->init_pointers();
			this->init_input();
		}

		void init_errors() noexcept
		{
			auto& font(m_datastore.get<sf::Font>("Fipps-Regular.otf"));

			// add the error instances
			m_errorhandler.create_error_instance(errors::cl_nullptr_access,
			"Something went wrong during program execution.\nPlease consider to make a bugreport at:\nhttps://github.com/Malekblubb/rectojump\n(Please attach 'log.log' and 'error.log'.)",
			font,
			[this]{this->set_only_state(state::error);});

			// TODO: add homepage here
			m_errorhandler.create_error_instance(errors::cl_data, "Some data wasn't loaded, can't run the game.\nPlease download a full release at: ",
			font,
			[this]{this->set_only_state(state::error);});


			// checking font
			if(!m_datamgr.exists_id("Fipps-Regular.otf"))
				mlk::exit_with("main font ('Fipps-Regular.otf') not loaded", mlk::lerr(errors::cl_data)["rj::game_handler"]);

			// checking data
			if(!m_datamgr.exists_ids({"debug_font.png", "Fipps-Regular.otf", "arrow.png",
									  "editor_item_rect.png", "editor_item_triangle.png", "editor_item_triangles4.png",
									  "menu_side.png"}))
				mlk::lerr(errors::cl_data);
		}

		void init_pointers() noexcept
		{
			m_game.set_levelmgr(&m_lvmgr);
		}

		void init_input()
		{
			// input update
			on_input_update() +=
			[this]{this->update_input();};

			// global input
			on_keys_pressed(key::LShift, key::F) +=
			[this]
			{
				m_game_window.toggle_fullscreen();
				rj::settings::set_fullscreen(m_game_window.get_fullscreen());
			};

			on_keys_pressed(key::LShift, key::T) +=
			[this]{m_game_window.toggle_titlebar();};

			on_keys_pressed(key::LShift, key::D) +=
			[this]{this->toggle_state(state::debug_info);};

			on_keys_pressed(key::LShift, key::Q) +=
			[this]{m_game_window.stop();};

			// window input
			on_key_pressed(key::Escape) +=
			[this]{if(!this->is_active(state::main_menu)) return; m_game_window.stop();};

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
			[this]{if(!this->is_active(state::game)) return; m_game.world().c_player();};
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
			if(this->is_active(state::error))
				return;

			m_backgroundmgr.update(duration);

			if(this->is_active(state::game))
				m_game.update(duration);

			else if(this->is_active(state::editor))
			{
				m_editor.update(duration);
				m_game.world().entityhandler().update(duration);
			}

			else if(this->is_active(state::main_menu))
				m_mainmenu.update(duration);

			if(this->is_active(state::debug_info))
				m_debug_info.update(duration);

			m_popupmgr.update(duration);
		}

		void update_input()
		{
			m_editor.update_input();
		}

		void render()
		{
			if(this->is_active(state::error))
			{
				m_render(m_errorhandler.get_current_error_instance().error_text);
				return;
			}

			m_default_camera.activate();
			m_backgroundmgr.render();

			m_default_camera.activate();
			// render game when in game menu
			if(this->is_active(state::game) || this->is_active(state::game_menu))
				m_game.render();

			else if(this->is_active(state::editor))
				m_editor.render();

			else if(this->is_active(state::main_menu))
				m_mainmenu.render();

			m_default_camera.activate();
			if(this->is_active(state::debug_info))
				m_debug_info.render();

			m_popupmgr.render();
		}

		// multi_args: true, Input_Type = key
		template<bool multi_args, typename Input_Type, state s, typename On_Input, typename... Input_Type_Args>
		struct add_input_helper
		{
			add_input_helper(const game_handler& gh, On_Input&& func, Input_Type_Args&&... keys)
			{
				on_keys_pressed(std::forward<Input_Type_Args>(keys)...) +=
				[&gh, func]{if(gh.is_active(s)) func();};
			}
		};

		// multi_args: false, Input_Type = key
		template<typename Input_Type, state s, typename On_Input, typename... Input_Type_Args>
		struct add_input_helper<false, Input_Type, s, On_Input, Input_Type_Args...>
		{
			add_input_helper(const game_handler& gh, On_Input&& func, Input_Type_Args&&... k)
			{
				on_key_pressed(std::forward<Input_Type_Args>(k)...) +=
				[&gh, func]{if(gh.is_active(s)) func();};
			}
		};

		// multi_args: false, Input_Type = btn
		template<state s, typename On_Input, typename... Input_Type_Args>
		struct add_input_helper<false, btn, s, On_Input, Input_Type_Args...>
		{
			add_input_helper(const game_handler& gh, On_Input&& func, Input_Type_Args&&... b)
			{
				on_btn_pressed(std::forward<Input_Type_Args>(b)...) +=
				[&gh, func](const vec2f& pos){if(gh.is_active(s)) func(pos);};
			}
		};

		// multi_args: false, Input_Type = wheel
		template<state s, typename On_Input, typename... Input_Type_Args>
		struct add_input_helper<false, wheel, s, On_Input, Input_Type_Args...>
		{
			add_input_helper(const game_handler& gh, On_Input&& func, Input_Type_Args&&... w)
			{
				on_mousewheel(std::forward<Input_Type_Args>(w)...) +=
				[&gh, func](const vec2f& pos){if(gh.is_active(s)) func(pos);};
			}
		};
	};
}



#endif // RJ_CORE_GAME_HANDLER_HPP
