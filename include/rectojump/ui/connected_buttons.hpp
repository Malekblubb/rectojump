//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_UI_CONNECTED_BUTTONS_HPP
#define RJ_UI_CONNECTED_BUTTONS_HPP


#include "button.hpp"
#include <rectojump/global/common.hpp>


namespace rj
{
	namespace ui
	{
		template<typename T>
		using btn_ptr = mlk::sptr<T>;

		using base_btn_ptr = btn_ptr<button>;

		class connected_buttons : public sf::Drawable
		{
			std::map<int, base_btn_ptr> m_buttons;
			int m_current_pressed_index{0};
			int m_current_add_index{0};

		public:
			mlk::slot<base_btn_ptr&> on_active_button;
			mlk::slot<base_btn_ptr&> on_inactive_button;

			connected_buttons() = default;

			void update(dur duration)
			{
				for(auto& a : m_buttons)
				{
					// update all buttons
					a.second->update(duration);

					// get current pressed button
					on_inactive_button(a.second);
					if(a.second->is_pressed())
						m_current_pressed_index = a.first;
				}
				// call the custom user settings
				if(m_current_pressed_index != -1)
					on_active_button(m_buttons[m_current_pressed_index]);
			}

			template<typename Button_Type, typename... Args>
			btn_ptr<Button_Type> add_button(Args&&... args)
			{
				auto ptr(std::make_shared<Button_Type>(std::forward<Args>(args)...));
				m_buttons.emplace(m_current_add_index, ptr);
				++m_current_add_index;
				return ptr;
			}

			void inactivate() noexcept
			{m_current_pressed_index = -1;}

			const base_btn_ptr& get_active_btn() const noexcept
			{return m_buttons.at(m_current_pressed_index);}

		private:
			void draw(sf::RenderTarget& target, sf::RenderStates states) const override
			{
				for(auto& a : m_buttons)
					target.draw(*a.second, states);
			}
		};
	}
}


#endif // RJ_UI_CONNECTED_BUTTONS_HPP
