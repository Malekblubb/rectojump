//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_CORE_MAIN_MENU_COMPONENT_MANAGER_HPP
#define RJ_CORE_MAIN_MENU_COMPONENT_MANAGER_HPP


#include "menu_component.hpp"
#include <rectojump/core/game_handler.hpp>

#include <mlk/containers/container_utl.h>

#include <memory>


namespace rj
{
	class game;

	template<typename T>
	using comp_ptr = std::shared_ptr<T>;

	template<typename Main_Menu>
	class component_manager
	{
		using mc_mm_ptr = comp_ptr<menu_component<Main_Menu>>;

		Main_Menu& m_mainmenu;
		game& m_game{m_mainmenu.m_game};
		const sf::Font& m_font{m_mainmenu.m_font};
		const vec2f& m_center{m_mainmenu.m_center};

		std::vector<mc_mm_ptr> m_components;

	public:
		component_manager(Main_Menu& mm) :
			m_mainmenu{mm}
		{ }

		void update(dur duration)
		{
			for(auto& a : m_components)
				if(m_mainmenu.is_active(a->get_type()))
					a->update(duration);
		}

		void render()
		{
			for(auto& a : m_components)
				if(m_mainmenu.is_active(a->get_type()))
					a->render();
		}

		template<typename Comp_Type, menu_state type, typename... Args>
		comp_ptr<Comp_Type> create_comp(Args&&... args)
		{
			auto ptr(std::make_shared<Comp_Type>(m_mainmenu, type, m_game, m_font, m_center, std::forward<Args>(args)...));
			m_components.emplace_back(ptr);
			return ptr;
		}
	};
}


#endif // RJ_CORE_MAIN_MENU_COMPONENT_MANAGER_HPP
