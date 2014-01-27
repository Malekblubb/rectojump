//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_EDITOR_BACKGROUND_EDITOR_HPP
#define RJ_GAME_EDITOR_BACKGROUND_EDITOR_HPP


#include <rectojump/game/background/background_manager.hpp>
#include <rectojump/global/config_settings.hpp>
#include <rectojump/shared/utils.hpp>


namespace rj
{
	template<typename Editor>
	class background_editor
	{
		Editor& m_editor;

		background_manager& m_backgroundmgr;

	public:
		background_editor(Editor& e) :
			m_editor{e},
			m_backgroundmgr{e.get_gamehandler().get_backgroundmgr()}
		{this->reset();}



		void reset()
		{
			m_backgroundmgr.clear();
		}
	};
}


#endif // RJ_GAME_EDITOR_BACKGROUND_EDITOR_HPP