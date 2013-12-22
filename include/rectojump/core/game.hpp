//
// Copyright (c) 2013 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_CORE_GAME_GAME_HPP
#define RJ_CORE_GAME_GAME_HPP


#include "game_updater.hpp"
#include "game_window.hpp"


namespace rj
{
	class game
	{
		game_window& m_window;

	public:
		game(game_window& window) :
			m_window{window}
		{
			m_window.updater().on_update.add_func([this](dur duration){this->update(duration);});
			m_window.updater().on_render.add_func([this]{this->render();});
		}

		void update(dur duration)
		{

		}

		void render()
		{

		}

		void render_object(const sf::Drawable& object)
		{m_window.draw(object);}
	};
}


#endif // RJ_CORE_GAME_GAME_HPP
