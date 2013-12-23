//
// Copyright (c) 2013 Christoph Malek
// See LICENSE for more information.
//

#include <rectojump/core/game.hpp>
#include <rectojump/core/game_window.hpp>
#include <rectojump/game/basic_entity.inl>


int main()
{
	rj::game_window gw;
	rj::game g{gw};

	gw.start();

	return 0;
}

