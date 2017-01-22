//
// Copyright (c) 2013-2017 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_MAIN_MENU_SITE_INVENTAR_HPP
#define RJ_GAME_MAIN_MENU_SITE_INVENTAR_HPP


namespace rj
{
	template<typename Overlay>
	class site_inventar
	{
		Overlay& m_overlay;

	public:
		site_inventar(Overlay& ov) :
			m_overlay{ov}
		{ }

		void construct()
		{

		}
	};
}


#endif // RJ_GAME_MAIN_MENU_SITE_INVENTAR_HPP






