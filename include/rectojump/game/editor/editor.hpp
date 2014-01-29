//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_EDITOR_EDITOR_HPP
#define RJ_GAME_EDITOR_EDITOR_HPP


#include "background_editor.hpp"
#include "editor_entity.hpp"
#include "itembar.hpp"
#include "mouse.hpp"
#include <rectojump/game/components/platfrom.hpp>
#include <rectojump/game/camera.hpp>
#include <rectojump/game/world.hpp>


namespace rj
{
	template<typename Game_Handler, typename Game>
	class editor
	{
		Game_Handler& m_gamehandler;
		Game& m_game;
		world& m_gameworld;
		entity_handler& m_entityhandler;

		camera m_editarea_camera;
		camera m_itembar_camera;

		static constexpr float m_itembar_height{100.f};
		editor_mouse m_mouse;
		background_editor<editor> m_background{*this};
		itembar<Game_Handler> m_itembar;

	public:
		editor(Game_Handler& gh) :
			m_gamehandler{gh},
			m_game{gh.get_game()},
			m_gameworld{m_game.get_world()},
			m_entityhandler{m_gameworld.get_entityhandler()},
			m_editarea_camera{m_gamehandler.get_gamewindow()},
			m_itembar_camera{m_gamehandler.get_gamewindow()},
			m_mouse{gh.get_render()},
			m_itembar{gh, {settings::get_window_size<vec2f>().x, m_itembar_height}}
		{this->init();}

		void update(dur duration)
		{
			// edit area
			m_editarea_camera.set_changes();
			m_mouse.update(duration);

			// itembar
			m_itembar_camera.set_changes();
			m_itembar.update(duration);
		}

		void render()
		{
			// edit area
			m_editarea_camera.set_changes();
			m_entityhandler.render();
			m_mouse.render();

			// itembar
			m_itembar_camera.set_changes();
			m_itembar.render();
		}

		void on_activate()
		{
			m_background.reset();
		}

		auto get_gamehandler() noexcept
		-> decltype(m_gamehandler)&
		{return m_gamehandler;}

	private:
		void init()
		{
			m_itembar.on_item_click = [this](base_btn_ptr& b){m_mouse.set_texture(b->get_texture());};

			this->init_input();
			this->init_cameras();
		}

		void init_input()
		{
			m_gamehandler.template add_input<state::editor>(
			[this](const vec2f& pos){this->on_mouse_left(pos);}, btn::Left);

			m_gamehandler.template add_input<state::editor>(
			[this](const vec2f& pos){this->on_mouse_right(pos);}, btn::Right);

			m_gamehandler.template add_input<state::editor>(
			[this](const vec2f& pos){m_editarea_camera.move({10.2f, 0.f});}, wheel::down);

			m_gamehandler.template add_input<state::editor>(
			[this](const vec2f& pos)
			{
				if(m_editarea_camera.get_center().x >= m_editarea_camera.get_startcenter().x)
					m_editarea_camera.move({-10.5f, 0.f});
			}, wheel::up);
		}

		void init_cameras() noexcept
		{
			auto window_size(settings::get_window_size<vec2f>());

			float itembar_top{(window_size.y - m_itembar_height) / window_size.y};
			float itembar_height{m_itembar_height / window_size.y};
			float editarea_height{(window_size.y - m_itembar_height) / window_size.y};


			sf::View editarea_view{vec2f{window_size.x, window_size.y - m_itembar_height} / 2.f, {window_size.x, window_size.y - m_itembar_height}};
			editarea_view.setViewport({0.f, 0.f, 1.f ,editarea_height});
			m_editarea_camera.set_view(editarea_view);

			sf::View itembar_view{vec2f{window_size.x, m_itembar_height} / 2.f, {window_size.x, m_itembar_height}};
			itembar_view.setViewport({0.f, itembar_top, 1.f, itembar_height});
			m_itembar_camera.set_view(itembar_view);
		}

		void on_mouse_left(const vec2f&)
		{
			auto mouse_bounds(bounds_from_vec(m_itembar_camera.get_mapped_mousepos()));
			if(mouse_bounds.intersects(m_itembar.get_bounds()))
				return;


			float f{48.f};
			vec2f new_pos{round_to(m_editarea_camera.get_mapped_mousepos().x, f), round_to(m_editarea_camera.get_mapped_mousepos().y, f)};

			if(m_mouse.get_texture())
			{
				auto ptr(m_gameworld.template create_entity<editor_entity>(new_pos));
				ptr->render_object().setTexture(m_mouse.get_texture());
			}
		}

		void on_mouse_right(const vec2f&)
		{
			m_itembar.deselect_all();
			m_mouse.clear();

			auto iter(m_entityhandler.exists_entity_at(m_editarea_camera.get_mapped_mousepos()));
			if(iter != std::end(m_entityhandler))
				m_entityhandler.delete_entity(iter);
		}
	};
}


#endif // RJ_GAME_EDITOR_EDITOR_HPP
