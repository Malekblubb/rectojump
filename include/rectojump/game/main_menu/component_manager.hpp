//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_CORE_MAIN_MENU_COMPONENT_MANAGER_HPP
#define RJ_CORE_MAIN_MENU_COMPONENT_MANAGER_HPP


#include "menu_component.hpp"

#include <mlk/types/types.h>


namespace rj
{
	template<typename T>
	using comp_ptr = mlk::sptr<T>;

	template<typename Main_Menu>
	class component_manager
	{
		using mc_mm_ptr = comp_ptr<menu_component<Main_Menu>>;

		Main_Menu& m_mainmenu;

		std::vector<mc_mm_ptr> m_components;

	public:
		component_manager(Main_Menu& mm) :
			m_mainmenu{mm}
		{ }

		template<typename Comp_Type, menu_state type, typename... Args>
		comp_ptr<Comp_Type> create_comp(Args&&... args)
		{
			auto ptr(std::make_shared<Comp_Type>(m_mainmenu, type, m_mainmenu.get_gamehandler().get_render(), m_mainmenu.get_font(), m_mainmenu.get_center(), std::forward<Args>(args)...));
			m_components.emplace_back(ptr);
			return ptr;
		}

		mc_mm_ptr get_comp_from_type(menu_state type)
		{
			for(auto& a : m_components)
				if(a->get_type() == type)
					return a;
			return nullptr;
		}
	};
}


#endif // RJ_CORE_MAIN_MENU_COMPONENT_MANAGER_HPP
