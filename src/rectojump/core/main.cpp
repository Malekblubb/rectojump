//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#include <rectojump/core/game.hpp>
#include <rectojump/core/game_window.hpp>
#include <rectojump/core/main_menu.hpp>
#include <rectojump/core/state.hpp>
#include <rectojump/game/debug_info.inl>
#include <rectojump/global/error_inserter.hpp>
#include <rectojump/shared/data_manager.hpp>


int main()
{
	// generate errors
	rj::error_inserter{};

	// data
	rj::data_manager dm{rj::data_path, true};

	// game
	rj::game_window gw;
	rj::game g{gw};
	rj::main_menu m{g};
	rj::state_handler sh{gw, g, m, dm};

	gw.start();
	return 0;
}
