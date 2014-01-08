//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_CORE_GAME_HANDLER_HPP
#define RJ_CORE_GAME_HANDLER_HPP


#include "game.hpp"
#include "game_window.hpp"
#include "main_menu.hpp"
#include <rectojump/shared/data_manager.hpp>

#include <mlk/containers/container_utl.h>
#include <mlk/tools/bitset.h>


namespace rj
{
	enum class state : char
	{main_menu, game_menu, game, debug_info, num};

	class game_handler
	{
		game_window& m_game_window;
		game& m_game;
		main_menu& m_main_menu;
		data_manager& m_datamgr;

		debug_info m_debug_info;

		mlk::bitset<state, mlk::enum_utl::to_int<std::size_t>(state::num)> m_current_states;

	public:
		game_handler(game_window& window, game& g, main_menu& menu, data_manager& datamgr) :
			m_game_window{window},
			m_game{g},
			m_main_menu{menu},
			m_datamgr{datamgr},
			m_debug_info{m_game}
		{
			m_current_states |= state::game;

			m_game_window.get_updater().on_update += [this](dur duration){this->update(duration);};
			m_game_window.get_updater().on_render += [this]{this->render();};


			on_keys_pressed(key::LShift, key::D) +=
			[this]{this->toggle_state(state::debug_info);};

			on_key_pressed(key::P) +=
			[this]
			{
				if(m_current_states & state::game)
				{
					m_current_states.remove(state::game);
					m_current_states |= state::game_menu;
					return;
				}

				if(m_current_states & state::game_menu)
				{
					m_current_states.remove(state::game_menu);
					m_current_states |= state::game;
					return;
				}
			};
		}

	private:
		void toggle_state(state s)
		{m_current_states.toggle(s);}

		bool is_state_activated(state s) const noexcept
		{return m_current_states & s;}

		void set_only_state(state s)
		{
			m_current_states.remove_all();
			m_current_states |= s;
		}

		void update(dur duration)
		{
			if(this->is_state_activated(state::game))
				m_game.update(duration);

			else if(this->is_state_activated(state::main_menu))
				m_main_menu.update(duration);

			if(this->is_state_activated(state::debug_info))
				m_debug_info.update(duration);
		}

		void render()
		{
			// render game when in game menu
			if(this->is_state_activated(state::game) || this->is_state_activated(state::game_menu))
				m_game.render();

			else if(this->is_state_activated(state::main_menu))
				m_main_menu.render();

			if(this->is_state_activated(state::debug_info))
				m_debug_info.render();
		}
	};
}



#endif // RJ_CORE_GAME_HANDLER_HPP
