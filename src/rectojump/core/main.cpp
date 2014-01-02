//
// Copyright (c) 2013 Christoph Malek
// See LICENSE for more information.
//

#include <rectojump/core/game.hpp>
#include <rectojump/core/game_window.hpp>
#include <rectojump/core/main_menu.hpp>
#include <rectojump/core/state.hpp>
#include <rectojump/global/error_inserter.hpp>
#include <rectojump/game/basic_entity.inl>
#include <rectojump/game/debug_info.inl>


int main()
{
	// generate errors
	rj::error_inserter{};

	// game
	rj::game_window gw;
	rj::game g{gw};
	rj::main_menu m;
	rj::state_handler sh{gw, g, m};

	gw.start();
	return 0;
}

