//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_EDITOR_BUTTON_ITEM_HPP
#define RJ_GAME_EDITOR_BUTTON_ITEM_HPP


#include <rectojump/ui/button.hpp>
#include <rectojump/global/config_settings.hpp>


namespace rj
{
	class button_item : public button
	{
	public:
		button_item(const vec2f& size, const vec2f& pos) :
			button{size, pos}
		{this->init_base();}

		void update(dur duration)
		{
			button::update(duration);
		}

	private:
		void init() override
		{
			m_shape.setFillColor(settings::get_color_dark());
			m_shape.setOutlineColor(settings::get_color_default_dark());
			m_shape.setOutlineThickness(2.f);

		}

		void on_hover() override
		{
			m_shape.setOutlineColor(settings::get_color_light());
		}
	};
}


#endif // RJ_GAME_EDITOR_BUTTON_ITEM_HPP