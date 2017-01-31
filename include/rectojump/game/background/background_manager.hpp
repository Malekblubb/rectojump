//
// Copyright (c) 2013-2017 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_BACKGROUND_BACKGROUND_MANAGER_HPP
#define RJ_GAME_BACKGROUND_BACKGROUND_MANAGER_HPP


#include "background_component.hpp"
#include "basic_background_component.hpp"
#include <rectojump/game/components/gradient_rect.hpp>

#include <mlk/containers/container_utl.h>
#include <mlk/types/types.h>


namespace rj
{
	template<typename Game_Handler>
	class background_manager
	{
		using bbc_ptr = mlk::sptr<basic_background_component>;
		rndr& m_render;
		Game_Handler& m_gamehandler;

		struct objects_per_state
		{
			gradient_rect bgshape;
			sf::RectangleShape textureshape;
			std::vector<bbc_ptr> components;
		};

		std::map<state, objects_per_state> m_objects;
	public:
		background_manager(rndr& r, Game_Handler& gh) :
			m_render{r},
			m_gamehandler{gh}
		{ }

		void update(dur duration)
		{
			auto do_update_for_state
			{
				[this, duration](state s)
				{
					// update all components of current state
					for(auto& a : m_objects[s].components)
						a->update(duration);

					// erase dead objects
					mlk::cnt::remove_all_if(
								[](const auto& p){return p->is_destroyed();},
					m_objects[s].components);
				}
			};

			if(m_gamehandler.states() & state::main_menu)
				do_update_for_state(state::main_menu);
			else if(m_gamehandler.states() & state::game)
				do_update_for_state(state::game);
			else if(m_gamehandler.states() & state::editor)
				do_update_for_state(state::editor);
			else if(m_gamehandler.states() & state::error)
				do_update_for_state(state::error);
		}

		void render()
		{
			auto do_render_for_state
			{
				[this](state s)
				{
					// render bg shape
					m_render(m_objects[s].bgshape);

					// update all components of current state
					for(auto& a : m_objects[s].components)
						a->render();

					// render texture shape
					m_render(m_objects[s].textureshape);
				}
			};

			if(m_gamehandler.states() & state::main_menu)
				do_render_for_state(state::main_menu);
			else if(m_gamehandler.states() & state::game)
				do_render_for_state(state::game);
			else if(m_gamehandler.states() & state::editor)
				do_render_for_state(state::editor);
			else if(m_gamehandler.states() & state::error)
				do_render_for_state(state::error);
		}

		void set_bg_shape(state s, const gradient_rect& shape) noexcept
		{m_objects[s].bgshape = shape;}

		void set_tx_shape(state s, const sf::RectangleShape& shape) noexcept
		{m_objects[s].textureshape = shape;}

		gradient_rect& bg_shape(state s) noexcept
		{return m_objects[s].bgshape;}

		const gradient_rect& get_bg_shape(state s) noexcept
		{return m_objects[s].bgshape;}

		void clear(state s) noexcept
		{
			auto& for_state{m_objects[s]};

			// clear components
			for_state.components.clear();

			// clear bg
			for_state.bgshape.set_gradient_points(1);
			for_state.bgshape.set_startcolor({});
			for_state.bgshape.set_endcolor({});
			for_state.textureshape.setFillColor({0, 0, 0, 0});
			for_state.textureshape.setTexture(nullptr);
		}

		std::size_t num_components_current_state() noexcept
		{return m_objects[m_gamehandler.current_renderable_state()].components.size();}

		template<typename Obj_Type, typename... Args>
		mlk::sptr<background_component<Obj_Type>> create_object_for_state(state s, Args&&... args)
		{
			auto ptr{std::make_shared<background_component<Obj_Type>>(m_render,
																	  std::forward<Args>(args)...)};
			m_objects[s].components.emplace_back(ptr);
			return ptr;
		}
	};
}



#endif // RJ_GAME_BACKGROUND_BACKGROUND_MANAGER_HPP
