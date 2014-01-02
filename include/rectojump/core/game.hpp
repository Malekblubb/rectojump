//
// Copyright (c) 2013 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_CORE_GAME_GAME_HPP
#define RJ_CORE_GAME_GAME_HPP


#include "game_updater.hpp"
#include "game_window.hpp"
#include <rectojump/game/debug_info.hpp>
#include <rectojump/game/world.hpp>


namespace rj
{
	class game
	{
		game_window& m_window;
		game_updater& m_updater;

		// gameworld
		world m_world{*this};

	public:
		game(game_window& window) :
			m_window{window},
			m_updater{m_window.get_updater()}
		{
//			m_updater.on_update += [this](dur duration){this->update(duration);};
//			m_updater.on_render += [this]{this->render();};
		}

		void update(dur duration)
		{
			m_world.update(duration);
		}

		void render()
		{
			m_world.render();
		}

		void render_object(const sf::Drawable& object)
		{m_window.draw(object);}

		game_updater& get_updater()
		{return m_updater;}

		world& get_world()
		{return m_world;}
	};
}


#endif // RJ_CORE_GAME_GAME_HPP
