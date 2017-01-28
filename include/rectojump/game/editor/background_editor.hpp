//
// Copyright (c) 2013-2017 Christoph Malek
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

		background_manager<typename Editor::gh_type>& m_backgroundmgr;

	public:
		background_editor(Editor& e) :
			m_editor{e},
			m_backgroundmgr{e.gamehandler().backgroundmgr()}
		{this->init();}

		void reset()
		{
			m_backgroundmgr.clear(state::editor);
		}

	private:
		void init()
		{
			m_backgroundmgr.set_bg_shape(state::editor, {settings::get_window_size<vec2f>()});
		}
	};
}


#endif // RJ_GAME_EDITOR_BACKGROUND_EDITOR_HPP
