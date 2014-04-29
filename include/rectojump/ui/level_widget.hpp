//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_UI_LEVEL_WIDGET_HPP
#define RJ_UI_LEVEL_WIDGET_HPP


#include "level_widget_item.hpp"
#include "widget.hpp"
#include <rectojump/core/game_window.hpp>
#include <rectojump/game/camera.hpp>
#include <rectojump/global/common.hpp>
#include <rectojump/global/config_settings.hpp>


namespace rj
{
	namespace ui
	{
		class level_widget : public widget
		{
			camera m_scrollcam;

			std::vector<level_widget_item> m_items;

		public:
			level_widget(game_window& gw, const vec2f& size = {}, const vec2f& pos = {}) :
				m_scrollcam{gw}
			{
				auto wsize(settings::get_window_size<vec2f>());

				sf::View view{size / 2.f, size};
				view.setViewport({pos.x / wsize.x, pos.y / wsize.y , size.x / wsize.x, size.y / wsize.y});

				m_scrollcam.set_view(view);
			}

			void update(dur) override
			{
				m_scrollcam.activate();
			}

			void activate_cam()
			{
				m_scrollcam.activate();
			}

			template<typename... Args>
			void add_item(Args&&... args)
			{m_items.emplace_back(std::forward<Args>(args)...);}

		private:
			void draw(sf::RenderTarget& target, sf::RenderStates states) const override
			{
				for(const auto& a : m_items)
					target.draw(a, states);
			}
		};
	}
}


#endif // RJ_UI_LEVEL_WIDGET_HPP
