//
// Copyright (c) 2013-2021 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_UI_STACKED_WIDGET_HPP
#define RJ_UI_STACKED_WIDGET_HPP

#include "widget.hpp"
#include <rectojump/game/camera.hpp>
#include <rectojump/global/common.hpp>
#include <rectojump/global/config_settings.hpp>

namespace rj
{
	namespace ui
	{
		struct site
		{
			camera cam;
			std::vector<mlk::sptr<ui::widget>> objects;
			mlk::slot<> on_render;
		};

		class stacked_widget : public sf::Drawable
		{
			game_window& m_gamewindow;

			vec2f m_size;
			vec2f m_pos;

			std::map<std::string, site> m_sites;
			std::string m_active;

		public:
			stacked_widget(game_window& gw, const vec2f& size = {0.f, 0.f},
						   const vec2f& pos = {0.f, 0.f})
				: m_gamewindow{gw}, m_size{size}, m_pos{pos}
			{
			}

			void update(dur duration)
			{
				if(m_active.empty()) return;

				this->activate_current_cam();

				for(const auto& a : m_sites.at(m_active).objects)
					a->update(duration);
			}

			void activate_current_cam()
			{
				if(!m_active.empty()) m_sites.at(m_active).cam.activate();
			}

			void set_size(const vec2f& size) noexcept { m_size = size; }

			void set_pos(const vec2f& pos) noexcept { m_pos = pos; }

			void add_site(const std::string& site_name)
			{
				auto size{settings::get_window_size<vec2f>()};
				m_sites.emplace(site_name,
								site{{m_gamewindow,
									  {m_size / 2.f, m_size},
									  {m_pos.x / size.x, m_pos.y / size.y,
									   m_size.x / size.x, m_size.y / size.y}}});
			}

			auto& get(const std::string& site_name)
			{
				return m_sites.at(site_name);
			}

			template <typename Any_Type, typename... Any_Args>
			mlk::sptr<Any_Type> add_object(const std::string& site_name,
										   Any_Args&&... args)
			{
				if(!mlk::cnt::exists_map_first(site_name, m_sites))
					return nullptr;

				auto ptr{std::make_shared<Any_Type>(
					std::forward<Any_Args>(args)...)};
				m_sites.at(site_name).objects.emplace_back(ptr);
				return ptr;
			}

			void switch_site(const std::string& site_name)
			{
				if(!mlk::cnt::exists_map_first(site_name, m_sites))
				{
					m_active.clear();
					return;
				}

				m_active = site_name;
			}

			const vec2f& size() const noexcept { return m_size; }

			const vec2f& pos() const noexcept { return m_pos; }

			sf::FloatRect bounds() const noexcept { return {m_pos, m_size}; }

			bool active(const std::string& site_name) const noexcept
			{
				return m_active == site_name;
			}

		private:
			virtual void draw(sf::RenderTarget& target,
							  sf::RenderStates states) const override
			{
				// call on_render event; set custom site camera, etc...
				m_sites.at(m_active).on_render();

				if(!m_active.empty())
					for(const auto& a : m_sites.at(m_active).objects)
						target.draw(*a, states);
			}
		};
	}// namespace ui
}// namespace rj

#endif// RJ_UI_STACKED_WIDGET_HPP
