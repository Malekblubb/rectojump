//
// Copyright (c) 2013-2017 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_MAIN_MENU_MAIN_MENU_HPP
#define RJ_GAME_MAIN_MENU_MAIN_MENU_HPP


#include "background_main_menu.hpp"
#include "overlay.hpp"
#include <rectojump/core/game_window.hpp>
#include <rectojump/core/render.hpp>
#include <rectojump/game/background/background_manager.hpp>
#include <rectojump/game/components/player.hpp>
#include <rectojump/game/factory.hpp>
#include <rectojump/global/common.hpp>
#include <rectojump/global/config_settings.hpp>
#include <rectojump/shared/level_manager/level_manager.hpp>
#include <rectojump/shared/data_manager.hpp>
#include <rectojump/shared/utils.hpp>
#include <rectojump/ui/stacked_widget.hpp>

#include <SFML/Graphics.hpp>


namespace rj
{
	template<typename Game_Handler>
	class main_menu
	{
    public:
        using gh_type = Game_Handler;
    private:
		Game_Handler& m_gamehandler;
		game_window& m_gamewindow;
		data_manager& m_datamgr;
		level_manager& m_lvmgr;
		data_store_type& m_datastore;
        background_manager<Game_Handler>& m_backgroundmgr;

		background_main_menu<main_menu> m_background;

		// overlay
        overlay<main_menu, Game_Handler> m_overlay;

	public:
		main_menu(Game_Handler& gh) :
			m_gamehandler{gh},
			m_gamewindow{gh.gamewindow()},
			m_datamgr{gh.datamgr()},
			m_lvmgr{gh.levelmgr()},
			m_datastore{gh.datastore()},
			m_backgroundmgr{gh.backgroundmgr()},
			m_background{*this},
			m_overlay{*this}
		{this->init();}

		void update(dur duration)
		{
			m_background.update(duration);
			m_overlay.update(duration);
		}

		void render()
		{
			m_overlay.render();
		}

		auto& gamehandler() noexcept
		{return m_gamehandler;}

		void on_activate()
		{

		}

	private:
		void init()
		{
			this->on_activate();


		}

		void init_input()
		{

		}
	};
}


#endif // RJ_GAME_MAIN_MENU_MAIN_MENU_HPP
