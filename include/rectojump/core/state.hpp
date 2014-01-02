//
// Copyright (c) 2013 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_CORE_STATE_HPP
#define RJ_CORE_STATE_HPP


#include "game.hpp"
#include "game_window.hpp"
#include "main_menu.hpp"

#include <mlk/containers/container_utl.h>
#include <mlk/tools/bitset.h>


namespace rj
{
	enum class state : char
	{main_menu, game_menu, game};

	class state_handler
	{
		game_window& m_game_window;
		game& m_game;
		main_menu& m_menu;
		debug_info m_debug_info;

		static constexpr std::size_t m_num_states{3};
		mlk::bitset<state, m_num_states> m_current_state;
		std::map<state, mlk::slot<dur>> m_states;


	public:
		state_handler(game_window& window, game& g, main_menu& menu) :
			m_game_window{window},
			m_game{g},
			m_menu{menu},
			m_debug_info{m_game}
		{
//			m_current_state |= state::main_menu;
			m_current_state |= state::game;
			m_states[state::game] += [this](dur duration){m_game.update(duration); m_game.render();};
			m_states[state::main_menu] += [this](dur duration){m_menu.update(duration);};

			m_game_window.get_updater().on_update += [this](dur duration){this->update(duration);};
			m_game_window.get_updater().on_render += [this]{this->render();};

			on_key_pressed(key::P) +=
			[this]
			{
				if(m_current_state & state::game)
				{this->set_only_state(state::game_menu); return;}

				if(m_current_state & state::game_menu)
				{this->set_only_state(state::game); return;}
			};
		}

	private:
		void set_only_state(state s)
		{
			m_current_state.remove_all();
			m_current_state |= s;
		}

		void update(dur duration)
		{
			for(auto& a : m_states)
				if(m_current_state & a.first)
					a.second(duration);

			m_debug_info.update(duration);
		}

		void render()
		{
			m_game.render();
			m_debug_info.render();
		}
	};
}



#endif // RJ_CORE_STATE_HPP
