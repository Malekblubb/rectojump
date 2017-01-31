//
// Copyright (c) 2013-2017 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_UI_CONNECTED_BUTTONS_HPP
#define RJ_UI_CONNECTED_BUTTONS_HPP


#include "button.hpp"
#include <rectojump/global/common.hpp>

#include <mlk/signals_slots/slot.h>


namespace rj
{
	namespace ui
	{
		template<typename T>
		using btn_ptr = mlk::sptr<T>;

		using base_btn_ptr = btn_ptr<button>;

		struct button_event
		{base_btn_ptr button; mlk::slot<> event;};

		class connected_buttons : public sf::Drawable
		{
			std::map<int, button_event> m_buttons;
			int m_current_pressed_index{0};
			int m_current_add_index{0};
			bool m_pressed{false};

		public:
			mlk::slot<base_btn_ptr&> on_press;
			mlk::slot<base_btn_ptr&> on_active_button;
			mlk::slot<base_btn_ptr&> on_inactive_button;

			connected_buttons() = default;

			void update(dur duration)
			{
				for(auto& a : m_buttons)
				{
					// update all buttons
					a.second.button->update(duration);

					// get current pressed button
					if(!a.second.button->pressed() && !a.second.button->hover())
						on_inactive_button(a.second.button);
					if(a.second.button->pressed())
					{
						m_current_pressed_index = a.first;
						if(!m_pressed)
						{
							a.second.event();
							on_press(m_buttons[m_current_pressed_index].button);
							m_pressed = true;
						}
					}
				}

				// call the custom user settings
				if(m_current_pressed_index != -1)
				{
					on_active_button(m_buttons[m_current_pressed_index].button);
					if(!m_buttons[m_current_pressed_index].button->pressed())
						m_pressed = false;
				}
			}

			template<typename Button_Type, typename... Args>
			btn_ptr<Button_Type> add_button(Args&&... args)
			{
				auto ptr{std::make_shared<Button_Type>(std::forward<Args>(args)...)};
				m_buttons.emplace(m_current_add_index, button_event{ptr, {}});
				++m_current_add_index;
				return ptr;
			}

			template<typename Button_Type, typename Func, typename... Args>
			btn_ptr<Button_Type> add_button_event(Func&& f, Args&&... args)
			{
				auto ptr{std::make_shared<Button_Type>(std::forward<Args>(args)...)};
				m_buttons.emplace(m_current_add_index, button_event{ptr, {f}});
				++m_current_add_index;
				return ptr;
			}

			void inactivate() noexcept
			{m_current_pressed_index = -1;}

			void set_active_button(int index)
			{m_current_pressed_index = index; on_press(m_buttons[m_current_pressed_index].button);}

			const base_btn_ptr& get_active_btn() const
			{return m_buttons.at(m_current_pressed_index).button;}

			auto& get_buttons()
			{return m_buttons;}

		private:
			void draw(sf::RenderTarget& target, sf::RenderStates states) const override
			{
				for(auto& a : m_buttons)
					target.draw(*a.second.button, states);
			}
		};
	}
}


#endif // RJ_UI_CONNECTED_BUTTONS_HPP
