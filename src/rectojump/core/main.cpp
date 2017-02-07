//
// Copyright (c) 2013-2017 Christoph Malek
// See LICENSE for more information.
//

#include <rectojump/core/game.hpp>
#include <rectojump/core/game_handler.hpp>
#include <rectojump/core/game_window.hpp>
#include <rectojump/game/editor/editor.hpp>
#include <rectojump/global/config_settings.hpp>
#include <rectojump/global/error_inserter.hpp>
#include <rectojump/shared/data_manager.hpp>
#include <rectojump/shared/error_handler.hpp>
#include <rectojump/shared/level_manager/level_manager.hpp>

int main()
{
	mlk::lerr_i().set_write_on_exit(true);
	mlk::lout("main", true) << "rectojump version " << rj::settings::version()
							<< " started";

	// init settings/config
	rj::settings::init();

	// generate errors
	rj::error_handler eh;
	rj::error_inserter{eh};

	// data
	rj::data_manager dm{rj::settings::data_path(),
						{"LICENSE.Fipps-Regular.pdf", "LICENSE.Ubuntu-Font"},
						true};
	rj::level_manager lm{rj::settings::level_path()};

	// game
	rj::game_window gw{rj::settings::get_window_size(),
					   rj::settings::get_fullscreen()};
	rj::game_handler gh{gw, eh, dm, lm};

	gw.start();
	return 0;
}
