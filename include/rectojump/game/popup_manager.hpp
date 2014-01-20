//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_POPUP_MANAGER_HPP
#define RJ_GAME_POPUP_MANAGER_HPP


#include "popup.hpp"
#include <rectojump/shared/data_manager.hpp>

#include <mlk/containers/container_utl.h>


namespace rj
{
	template<typename Game_Handler>
	class popup_manager
	{
		game& m_game;
		sf::Font m_font;

		std::vector<popup> m_popups;

	public:
		popup_manager(Game_Handler& gh) :
			m_game{gh.get_game()},
			m_font{gh.get_datamgr().template get_as<sf::Font>("Fipps-Regular.otf")}
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

		std::size_t num_popups() const noexcept
		{return m_popups.size();}

	private:
		void erase_destroyed() noexcept
		{
			mlk::cnt::remove_all_if(
			[](const popup& p){return p.is_destroyed();}, m_popups);
		}
	};
}



#endif // RJ_GAME_POPUP_MANAGER_HPP
