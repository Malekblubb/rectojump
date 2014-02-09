//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_EDITOR_MOUSE_HPP
#define RJ_GAME_EDITOR_MOUSE_HPP


#include "editor_entity.hpp"
#include <rectojump/game/camera.hpp>
#include <rectojump/game/entity_handler.hpp>
#include <rectojump/global/config_settings.hpp>
#include <rectojump/shared/input.hpp>


namespace rj
{
	template<typename Game_Handler>
	class editor_mouse
	{
		rndr& m_render;
		entity_handler& m_entityhandler;

		// selection
		sf::RectangleShape m_selection_shape;
		std::vector<editor_entity> m_copy_shapes;

		// mouse 'indicator' shape
		editor_entity m_mouse_shape{{0.f, 0.f}};

		bool m_mouse_visible{false}, m_selection_visible{false}, m_selecting{false};

	public:
		editor_mouse(Game_Handler& gh) :
			m_render{gh.get_render()},
			m_entityhandler{gh.get_game().get_world().get_entityhandler()}
		{this->init();}

		void update(dur)
		{
			auto pos(this->get_rounded_mousepos());

			// mouse visible
			if(m_mouse_visible)
				m_mouse_shape.render_object().setPosition(pos);

			// selecting
			else if(m_selecting)
			{
				auto new_size(pos - m_selection_shape.getPosition());
				m_selection_shape.setSize(new_size);

				if(!is_btn_pressed(btn::Left))
					this->selection_end();
			}

			for(auto& a : m_copy_shapes)
			{
				auto offset(pos - m_selection_shape.getPosition());
				a.render_object().move(offset);
			}

			if(!m_selecting && m_selection_visible)
				m_selection_shape.setPosition(pos);
		}

		void render()
		{
			if(m_selection_visible || m_selecting)
			{
				m_render(m_selection_shape);
				for(auto& a : m_copy_shapes)
					a.render();
			}

			if(m_mouse_visible)
				m_render(m_mouse_shape.get_render_object());
		}

		void selection_start()
		{
			m_selection_shape.setPosition(this->get_rounded_mousepos());
			m_selecting = true;
			m_selection_visible = true;
			m_mouse_visible = false;
		}

		void selection_end()
		{
			m_selecting = false;
			m_copy_shapes.clear();
			auto selectshape_bounds(m_selection_shape.getGlobalBounds());
			for(auto& a : m_entityhandler)
			{
				auto size(a->size());
				auto ent_bounds(bounds_from_vec({a->pos().x - size.x / 2.f, a->pos().y - size.y / 2.f}, size));
				if(selectshape_bounds.intersects(ent_bounds))
					m_copy_shapes.push_back(*std::static_pointer_cast<editor_entity>(a));
			}
		}

		void clear() noexcept
		{
			m_mouse_shape.render_object().setTexture(nullptr);
			m_copy_shapes.clear();
			m_selection_visible = false;
			m_selecting = false;
			m_mouse_visible = false;
		}

		void deactivate_selection() noexcept
		{m_selection_visible = false; m_selecting = false;}

		void set_mouse_visible(bool b)
		{m_mouse_visible = b;}

		void set_texture(const sf::Texture* tx) noexcept
		{m_mouse_shape.render_object().setTexture(tx);}

		const sf::Texture* get_texture() noexcept
		{return m_mouse_shape.render_object().getTexture();}

		auto get_selected()
		-> const decltype(m_copy_shapes)&
		{return m_copy_shapes;}

		sf::FloatRect get_selectionshape_bounds() const noexcept
		{return m_selection_shape.getGlobalBounds();}

		bool is_selection_visible() const noexcept
		{return m_selection_visible;}

		bool is_mouse_visible() const noexcept
		{return m_mouse_visible;}

	private:
		void init()
		{
			m_selection_shape.setOutlineThickness(1);
			m_selection_shape.setOutlineColor(sf::Color::White);
			m_selection_shape.setFillColor({255, 255, 255, 50});
		}

		vec2f get_rounded_mousepos() const noexcept
		{
			auto pos(get_mousepos_current_view());
			return {round_to(pos.x, 48.f), round_to(pos.y, 48.f)};
		}
	};
}


#endif // RJ_GAME_EDITOR_MOUSE_HPP
