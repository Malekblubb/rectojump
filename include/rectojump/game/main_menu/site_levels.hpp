//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_MAIN_MENU_SITE_LEVELS_HPP
#define RJ_GAME_MAIN_MENU_SITE_LEVELS_HPP


namespace rj
{
	template<typename Overlay>
	class site_levels
	{
		Overlay& m_overlay;

	public:
		site_levels(Overlay& ov) :
			m_overlay{ov}
		{ }

		void construct()
		{

		}
	};
}


#endif // RJ_GAME_MAIN_MENU_SITE_LEVELS_HPP





