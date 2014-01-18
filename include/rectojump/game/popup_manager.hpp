//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_POPUP_MANAGER_HPP
#define RJ_GAME_POPUP_MANAGER_HPP


#include "popup.hpp"
#include <rectojump/shared/data_manager.hpp>


namespace rj
{
	class popup_manager
	{
		game& m_game;
		sf::Font m_font;

		std::vector<popup> m_popups;

	public:
		popup_manager(game& g, data_manager& dm) :
			m_game{g},
			m_font{dm.get_as<sf::Font>("Fipps-Regular.otf")}
		{ }

		void update(dur duration)
		{
			for(auto& a : m_popups)
				a.update(duration);
			this->erase_destroyed();
		}

		void render()
		{
			for(auto& a : m_popups)
				a.render();
		}

		template<typename... Args>
		void create_popup(const std::string& text, Args&&... args)
		{m_popups.emplace_back(&m_game, m_font, text, std::forward<Args>(args)...);}

	private:
		void erase_destroyed() noexcept
		{
			m_popups.erase(std::remove_if(std::begin(m_popups), std::end(m_popups),
			[](const popup& p){return p.is_destroyed();}), std::end(m_popups));
		}
	};
}



#endif // RJ_GAME_POPUP_MANAGER_HPP
