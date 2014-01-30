//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_EDITOR_ITEMBAR_HPP
#define RJ_GAME_EDITOR_ITEMBAR_HPP


#include "button_item.hpp"
#include <rectojump/global/common.hpp>
#include <rectojump/global/config_settings.hpp>
#include <rectojump/shared/data_manager.hpp>
#include <rectojump/shared/utils.hpp>
#include <rectojump/ui/connected_buttons.hpp>


namespace rj
{
	template<typename Game_Handler>
	class itembar
	{
		Game_Handler& m_gamehandler;
		rndr& m_render;
		data_manager& m_datamgr;

		sf::RectangleShape m_shape;
		std::vector<sf::Texture> m_button_textures;
		connected_buttons m_buttons;
		const vec2f m_buttonsize{64.f, 64.f};

	public:
		mlk::slot<base_btn_ptr&> on_item_click;

		itembar(Game_Handler& gh, const vec2f& size) :
			m_gamehandler{gh},
			m_render{gh.get_render()},
			m_datamgr{gh.get_datamgr()},
			m_shape{size},
			m_button_textures{m_datamgr.get_all_containing_as<sf::Texture>("editor_item")}
		{this->init();}

		void update(dur duration)
		{
			m_buttons.update(duration);
		}

		void render()
		{m_render(m_shape, m_buttons);}

		void deselect_all() noexcept
		{m_buttons.inactivate();}

		const sf::Texture& get_current_texture() const noexcept
		{return *m_buttons.get_active_btn()->get_texture();}

		auto get_bounds() const noexcept
		-> decltype(m_shape.getGlobalBounds())
		{return m_shape.getGlobalBounds();}

		const vec2f& get_size() const noexcept
		{return m_shape.getSize();}

	private:
		void init()
		{
			auto& rec_size(m_shape.getSize());
			m_shape.setOrigin(rec_size / 2.f);
			m_shape.setPosition(rec_size / 2.f);
			m_shape.setFillColor(settings::get_color_default_light());

			auto pos_x(150.f);
			for(auto& a : m_button_textures)
			{
				auto ptr(m_buttons.add_button<button_item>(m_buttonsize, vec2f{pos_x, m_shape.getPosition().y}));
				ptr->set_origin(m_buttonsize / 2.f);
				ptr->set_texture(&a);
				pos_x += 100.f;
			}

			m_buttons.on_active_button =
			[this](base_btn_ptr& b)
			{
				b->set_color(settings::get_color_light());
				b->set_outlinecolor(settings::get_color_light());
				on_item_click(b);
			};

			m_buttons.on_inactive_button =
			[](base_btn_ptr& b)
			{
				b->set_color(settings::get_color_default_dark());
				b->set_outlinecolor(settings::get_color_default_dark());
			};
		}
	};
}


#endif // RJ_GAME_EDITOR_ITEMBAR_HPP
