//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_CORE_MAIN_MENU_MENU_HPP
#define RJ_CORE_MAIN_MENU_MENU_HPP


#include "basic_component.hpp"
#include "items.hpp"
#include "level_squares.hpp"
#include <rectojump/global/common.hpp>

#include <mlk/signals_slots/slot.h>
#include <mlk/types/types.h>

#include <stack>


namespace rj
{
	template<typename State_Enum, State_Enum start_state>
	class submenu_manager
	{
		using bc_ptr = mlk::sptr<basic_component>;

		std::stack<State_Enum> m_state_stack;
		std::map<State_Enum, bc_ptr> m_components;

	public:
		submenu_manager()
		{m_state_stack.push(start_state);}

		void update_current_state(dur duration)
		{this->current()->update(duration);}

		void render_current_state()
		{this->current()->render();}

		void event_up()
		{this->current()->on_key_up();}

		void event_down()
		{this->current()->on_key_down();}

		void event_current()
		{this->current()->call_current_event();}

		template<typename Basic_Component_Ptr>
		void add_menu(State_Enum state, const Basic_Component_Ptr& bc)
		{m_components[state] = bc;}

		void switch_state(State_Enum new_state) noexcept
		{m_state_stack.push(new_state);}

		void activate_prev_state() noexcept
		{
			if(m_state_stack.size() == 1)
				return;
			m_state_stack.pop();
		}

		State_Enum get_current_state() const noexcept
		{return m_state_stack.top();}

	private:
		bc_ptr& current() noexcept
		{return m_components[this->get_current_state()];}
	};
}



#endif // RJ_CORE_MAIN_MENU_MENU_HPP
