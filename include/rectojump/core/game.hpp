//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_CORE_GAME_GAME_HPP
#define RJ_CORE_GAME_GAME_HPP


#include "game_updater.hpp"
#include "game_window.hpp"
#include <rectojump/game/debug_info.hpp>
#include <rectojump/game/world.hpp>
#include <rectojump/global/common.hpp>
#include <rectojump/shared/level_manager/level_manager.hpp>


namespace rj
{
	template<typename Game_Handler>
	class game
	{
		Game_Handler& m_gamehandler;
		level_manager* m_lvmgr{nullptr};

		// gameworld
		world m_world;

	public:
		game(Game_Handler& gh) :
			m_gamehandler{gh},
			m_world{m_gamehandler.get_render()}
		{ }

		void update(dur duration)
		{
			m_world.update(duration);
		}

		void render()
		{
			m_world.render();
		}

		void load_level(const level& lv)
		{
			if(m_lvmgr == nullptr)
				mlk::lerr(errors::cl_nullptr_access)["rj::game"];

			m_world.load_level(lv.entities);
		}

		void set_levelmgr(level_manager* lm)
		{m_lvmgr = lm;}

		world& get_world()
		{return m_world;}
	};
}


#endif // RJ_CORE_GAME_GAME_HPP
