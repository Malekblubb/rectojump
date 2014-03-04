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
	enum class popup_type : char
	{normal, info, error};

	template<popup_type, typename>
	struct popup_creator;

	template<typename Game_Handler>
	class popup_manager
	{
		Game_Handler& m_gamehandler;

		sf::Font m_font;

		std::vector<popup> m_popups;

	public:
		popup_manager(Game_Handler& gh) :
			m_gamehandler{gh},
			m_font{gh.datamgr().template get_as<sf::Font>("Fipps-Regular.otf")}
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

		template<popup_type type = popup_type::normal, typename... Args>
		void create_popup(const std::string& text, const vec2f& pos = {settings::get_window_size<vec2f>().x / 2.f, settings::get_window_size<vec2f>().y - 100.f}, Args&&... args = {})
		{popup_creator<type, Game_Handler>{*this, text, pos, std::forward<Args>(args)...};}

		template<typename... Args>
		void create_popup_impl(Args&&... args)
		{m_popups.emplace_back(&m_gamehandler.rendermgr(), m_font, std::forward<Args>(args)...);}

		std::size_t num_popups() const noexcept
		{return m_popups.size();}

	private:
		void erase_destroyed() noexcept
		{
			mlk::cnt::remove_all_if(
			[](const auto& p){return p.is_destroyed();}, m_popups);
		}
	};

	template<popup_type type, typename Game_Handler>
	struct popup_creator
	{
		template<typename... Args>
		popup_creator(popup_manager<Game_Handler>& pm, Args&&... args)
		{pm.create_popup_impl(std::forward<Args>(args)...);}
	};

	template<typename Game_Handler>
	struct popup_creator<popup_type::info, Game_Handler>
	{
		popup_creator(popup_manager<Game_Handler>& pm, const std::string& text, const vec2f& pos)
		{pm.create_popup_impl(text, pos, 3000, to_rgb("#000375", 200), to_rgb("#000352"), to_rgb("#e3e3e3"));}
	};

	template<typename Game_Handler>
	struct popup_creator<popup_type::error, Game_Handler>
	{
		popup_creator(popup_manager<Game_Handler>& pm, const std::string& text, const vec2f& pos)
		{pm.create_popup_impl(text, pos, 3000, to_rgb("#820006", 200), to_rgb("#d80042"), to_rgb("#e3e3e3"));}
	};
}



#endif // RJ_GAME_POPUP_MANAGER_HPP
