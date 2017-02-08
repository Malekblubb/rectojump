//
// Copyright (c) 2013-2017 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_WORLD_HPP
#define RJ_GAME_WORLD_HPP

#include "components/platform.hpp"
#include "components/player.hpp"
#include "components/triangle.hpp"
#include "components/triangles4.hpp"
#include "entity_handler.hpp"
#include "factory.hpp"
#include <rectojump/game/background/background_manager.hpp>
#include <rectojump/game/editor/editor_entity.hpp>
#include <rectojump/shared/data_store.hpp>

#include <mlk/tools/random_utl.h>

namespace rj
{
	template <typename Game_Handler>
	class world
	{
		Game_Handler& m_gamehandler;
		background_manager<Game_Handler>& m_backgroundmgr;
		data_store_type& m_datastore;

		entity_handler m_entity_handler;

	public:
		world(Game_Handler& gh)
			: m_gamehandler{gh},
			  m_backgroundmgr{gh.backgroundmgr()},
			  m_datastore{gh.datastore()},
			  m_entity_handler{gh.rendermgr(), gh.particlemgr()}
		{
			// init background
			m_backgroundmgr.bg_shape(state::game)
				.set_size(settings::get_window_size<vec2f>());
		}

		void c_player()
		{
			// TODO: check this
			auto first_ent{
				std::static_pointer_cast<platform>(*m_entity_handler.begin())};
			auto plr{factory::create<player>(vec2f{100.f, first_ent->top_out()},
											 first_ent->top_out())};
			m_entity_handler.create_entity(plr);
			m_entity_handler.player()->on_spawn();
		}

		void update(dur duration) { m_entity_handler.update(duration); }

		void render() { m_entity_handler.render(); }

		template <bool load_editor>
		void load_level(const entity_proto_vec& entities)
		{
			m_entity_handler.clear();

			for(auto& entity : entities) {
				vec2f pos{entity[x], entity[y]};
				auto ef{static_cast<char>(entity[figure])};

				// laod objects
				// rectangle
				if(ef == entity_figure::f_rectangle) {
					// we need other entity type when editing
					if
						constexpr(load_editor)
						{
							auto a{this->create_entity<editor_entity>(pos)};
							a->set_texture(
								&m_datastore.template get<sf::Texture>(
									"editor_item_rect.png"));
							a->set_figure(static_cast<entity_figure>(ef));
						}
					else
					{
						auto a{this->create_entity<platform>(
							pos, vec2f{48.f, 48.f})};
						a->render_object().setTexture(
							&m_datastore.template get<sf::Texture>(
								"editor_item_rect.png"));
					}
				}

				// triangles
				else if(ef == entity_figure::f_triangle)
				{
					// we need other entity type when editing
					if
						constexpr(load_editor)
						{
							auto a{this->create_entity<editor_entity>(pos)};
							a->set_texture(
								&m_datastore.template get<sf::Texture>(
									"editor_item_triangle.png"));
							a->set_figure(static_cast<entity_figure>(ef));
						}
					else
					{
						auto a{this->create_entity<platform>(
							pos, vec2f{48.f, 48.f})};
						a->render_object().setTexture(
							&m_datastore.template get<sf::Texture>(
								"editor_item_triangle.png"));
						a->set_propertie(entity_propertie::death);
					}
				}
			}

			this->c_player();
		}

		template <typename Entity_Type, typename... Args>
		entity_ptr<Entity_Type> create_entity(Args&&... args)
		{
			auto ptr{factory::create<Entity_Type>(std::forward<Args>(args)...)};
			m_entity_handler.create_entity(ptr);
			return ptr;
		}

		std::size_t num_entities() const noexcept
		{
			return m_entity_handler.num_entities();
		}

		entity_handler& entityhandler() noexcept { return m_entity_handler; }

	private:
	};
}

#endif// RJ_GAME_WORLD_HPP
