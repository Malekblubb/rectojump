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
	class game
	{
		game_window& m_window;
		level_manager* m_lvmgr{nullptr};

		// gameworld
		world m_world{*this};

	public:
		game(game_window& window) :
			m_window{window}
		{ }

		void update(dur duration)
		{
			m_world.update(duration);
		}

		void render()
		{
			m_world.render();
		}

		void load_level(const level_id& id)
		{
			if(m_lvmgr == nullptr)
				mlk::lerr(errors::cl_nullptr_access)["rj::game"];

			auto& lv(m_lvmgr->get_level(id));
			m_world.load_level(lv.entities);
		}

		void render_object(const sf::Drawable& object)
		{m_window.draw(object);}

		void set_levelmgr(level_manager* lm)
		{m_lvmgr = lm;}

		world& get_world()
		{return m_world;}
	};
}


#endif // RJ_CORE_GAME_GAME_HPP
