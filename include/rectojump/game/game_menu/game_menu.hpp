//
// Copyright (c) 2013-2017 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_GAME_MENU_GAME_MENU_HPP
#define RJ_GAME_GAME_MENU_GAME_MENU_HPP


#include <rectojump/global/common.hpp>


namespace rj
{
	template<typename Game_Handler>
	class game_menu
	{
		Game_Handler& m_gamehandler;

	public:
		game_menu(Game_Handler& gh) :
			m_gamehandler{gh}
		{this->init();}

		void update(dur duration)
		{

		}

		void render()
		{

		}

	private:
		void init()
		{

		}
	};
}


#endif // RJ_GAME_GAME_MENU_GAME_MENU_HPP
