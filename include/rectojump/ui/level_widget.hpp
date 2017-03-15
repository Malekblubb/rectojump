//
// Copyright (c) 2013-2017 Christoph Malek
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
		class level_widget final : public widget
		{
			// cam
			camera m_scrollcam;

			// properties
			vec2f m_size;
			vec2f m_pos;

			// scrolling
			static constexpr float m_spacing{10.f};
			float m_full_height{0.f};

			// items
			std::vector<level_widget_item> m_items;

		public:
			level_widget(game_window& gw, const vec2f& size = {},
						 const vec2f& pos = {})
				: m_scrollcam{gw}, m_size{size}, m_pos{pos}
			{
				auto wsize{settings::get_window_size<vec2f>()};

				sf::View view{size / 2.f, size};
				view.setViewport({pos.x / wsize.x, (pos.y + 2.f) / wsize.y,
								  size.x / wsize.x, size.y / wsize.y});

				m_scrollcam.set_view(view);
			}

			void update(dur duration) override
			{
				this->activate_cam();

				for(auto& a : m_items) a.update(duration);
			}

			void activate_cam() { m_scrollcam.activate(); }

			void scroll_up(float step = -15.f)
			{
				if(!m_scrollcam.has_moved_down()) return;

				m_scrollcam.move({0.f, step});
			}

			void scroll_down(float step = 15.f)
			{
				if(m_scrollcam.get_center().y >= m_full_height) return;

				m_scrollcam.move({0.f, step});
			}
			template <typename Func_Type1, typename Func_Type2>
			void add_item(const level& lv, const sf::Font& font,
						  Func_Type1&& fun_play, Func_Type2&& fun_edit)
			{
				if(!lv.is_valid()) return;

				// add item
				m_items.emplace_back(lv, font, m_size.x);

				// move the next item under the previous
				float pos_y{0.f};
				if(m_items.size() > 1)
					pos_y = m_items[m_items.size() - 2].pos_y_h();

				m_items.back().move({0.f, pos_y + m_spacing});

				// set events
				m_items.back().on_play_clicked() = fun_play;
				m_items.back().on_edit_clicked() = fun_edit;

				// calc full height
				m_full_height = -m_scrollcam.get_startcenter().y;
				for(const auto& a : m_items)
					m_full_height += a.size().y + m_spacing;
			}

		private:
			void draw(sf::RenderTarget& target,
					  sf::RenderStates states) const override
			{
				for(const auto& a : m_items) target.draw(a, states);
			}
		};
	}
}

#endif// RJ_UI_LEVEL_WIDGET_HPP
