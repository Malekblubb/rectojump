//
// Copyright (c) 2013-2017 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_MAIN_MENU_SITE_SCORES_HPP
#define RJ_GAME_MAIN_MENU_SITE_SCORES_HPP


namespace rj
{
	template<typename Overlay>
	class site_scores
	{
		Overlay& m_overlay;

	public:
		site_scores(Overlay& ov) :
			m_overlay{ov}
		{ }

		void construct()
		{

		}
	};
}


#endif // RJ_GAME_MAIN_MENU_SITE_SCORES_HPP




