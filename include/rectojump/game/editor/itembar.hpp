//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_EDITOR_ITEMBAR_HPP
#define RJ_GAME_EDITOR_ITEMBAR_HPP


#include "button_item.hpp"
#include <rectojump/game/entity_groups.hpp>
#include <rectojump/global/common.hpp>
#include <rectojump/global/config_settings.hpp>
#include <rectojump/shared/data_manager.hpp>
#include <rectojump/shared/data_store.hpp>
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
		typename Game_Handler::data_store_type& m_datastore;

		sf::RectangleShape m_shape;
		std::vector<std::reference_wrapper<sf::Texture>> m_button_textures;
		ui::connected_buttons m_buttons;
		const vec2f m_buttonsize{64.f, 64.f};

	public:
		mlk::slot<ui::base_btn_ptr&> on_item_click;

		itembar(Game_Handler& gh, const vec2f& size) :
			m_gamehandler{gh},
			m_render{gh.rendermgr()},
			m_datamgr{gh.datamgr()},
			m_datastore{gh.datastore()},
			m_shape{size}
		{this->init();}

		void update(dur duration)
		{
			m_buttons.update(duration);
		}

		void render()
		{m_render(m_shape, m_buttons);}

		void select(int index) noexcept
		{m_buttons.set_active_button(index);}

		void deselect_all() noexcept
		{m_buttons.inactivate();}

		const sf::Texture& get_current_texture() const noexcept
		{return *m_buttons.get_active_btn()->get_texture();}

		entity_figure get_current_figure() const noexcept
		{return std::static_pointer_cast<button_item>(m_buttons.get_active_btn())->get_figure();}

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

			m_button_textures.emplace_back(m_datastore.template get<sf::Texture>("editor_item_rect.png"));
			m_button_textures.emplace_back(m_datastore.template get<sf::Texture>("editor_item_triangle.png"));
			m_button_textures.emplace_back(m_datastore.template get<sf::Texture>("editor_item_triangles4.png"));

			auto pos_x(150.f);
			std::size_t index{0};
			for(auto& a : m_button_textures)
			{
				auto rect_ptr(m_buttons.add_button<button_item>(m_buttonsize, vec2f{pos_x, m_shape.getPosition().y}));
				rect_ptr->set_origin(m_buttonsize / 2.f);
				rect_ptr->set_texture(&a.get());
				rect_ptr->set_figure(static_cast<entity_figure>(index));
				pos_x += 100.f;
				++index;
			}

			m_buttons.on_active_button =
			[this](ui::base_btn_ptr& b)
			{
				b->set_color(settings::get_color_light());
				b->set_outlinecolor(settings::get_color_light());
			};

			m_buttons.on_inactive_button =
			[](ui::base_btn_ptr& b)
			{
				b->set_color(settings::get_color_default_dark());
				b->set_outlinecolor(settings::get_color_default_dark());
			};

			m_buttons.on_press = [this](ui::base_btn_ptr& b){on_item_click(b);};
		}
	};
}


#endif // RJ_GAME_EDITOR_ITEMBAR_HPP
