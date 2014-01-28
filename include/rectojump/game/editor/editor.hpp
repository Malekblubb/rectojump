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

		editor_mouse m_mouse;
		background_editor<editor> m_background{*this};
		itembar<Game_Handler> m_itembar;

	public:
		editor(Game_Handler& gh) :
			m_gamehandler{gh},
			m_game{gh.get_game()},
			m_gameworld{m_game.get_world()},
			m_entityhandler{m_gameworld.get_entityhandler()},
			m_mouse{gh.get_render()},
			m_itembar{gh}
		{this->init();}

		void update(dur duration)
		{
			m_mouse.update(duration);
			m_itembar.update(duration);
		}

		void render()
		{
			m_mouse.render();
			m_itembar.render();
		}

		void on_activate()
		{
			m_background.reset();
		}

		void on_mouse_left(const vec2f& pos)
		{
			auto mouse_bounds(get_mousebounds());
			if(mouse_bounds.intersects(m_itembar.get_bounds()))
				return;


			float f{48.f};
			vec2f new_pos{round_to(pos.x, f), round_to(pos.y, f)};


			auto ptr(m_gameworld.template create_entity<editor_entity>(new_pos));
			ptr->render_object().setTexture(m_mouse.get_texture());
		}

		void on_mouse_right(const vec2f& pos)
		{
			m_itembar.deselect_all();
			m_mouse.clear();

			auto iter(m_entityhandler.exists_entity_at(pos));
			if(iter != std::end(m_entityhandler))
				m_entityhandler.delete_entity(iter);
		}

		void on_mousewheel()
		{

		}

		auto get_gamehandler() noexcept
		-> decltype(m_gamehandler)&
		{return m_gamehandler;}

	private:
		void init()
		{
			m_itembar.on_item_click = [this](base_btn_ptr& b){m_mouse.set_texture(b->get_texture());};

			this->init_input();
		}

		void init_input()
		{
			m_gamehandler.template add_input<state::editor>(
			[this](const vec2f& pos){this->on_mouse_left(pos);}, btn::Left);

			m_gamehandler.template add_input<state::editor>(
			[this](const vec2f& pos){this->on_mouse_right(pos);}, btn::Right);
		}
	};
}



#endif // RJ_GAME_EDITOR_EDITOR_HPP
