//
// Copyright (c) 2013-2021 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_MAIN_MENU_SITE_SETTINGS_HPP
#define RJ_GAME_MAIN_MENU_SITE_SETTINGS_HPP


namespace rj
{
	template<typename Overlay>
	class site_settings
	{
		Overlay& m_overlay;

	public:
		site_settings(Overlay& ov) :
			m_overlay{ov}
		{ }

		void construct()
		{

		}
	};
}


#endif // RJ_GAME_MAIN_MENU_SITE_SETTINGS_HPP



