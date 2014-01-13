//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#include <rectojump/core/game.hpp>
#include <rectojump/core/game_handler.hpp>
#include <rectojump/core/game_window.hpp>
#include <rectojump/game/debug_info.inl>
#include <rectojump/game/main_menu/main_menu.hpp>
#include <rectojump/global/config_settings.hpp>
#include <rectojump/global/error_inserter.hpp>
#include <rectojump/shared/level_manager/level_manager.hpp>
#include <rectojump/shared/data_manager.hpp>


int main()
{
	mlk::lout("main", true) << "rectojump version " << rj::get_version() << " started";

	// init settings/config
	rj::settings::init();

	// generate errors
	rj::error_inserter{};

	// data
	rj::data_manager dm{rj::data_path, true};
	rj::level_manager lm{rj::level_path};

	// game
	rj::game_window gw;
	rj::game g{gw};
	rj::main_menu m{g, gw, dm, lm};
	rj::game_handler gh{gw, g, m, dm, lm};

	gw.start();
	return 0;
}
