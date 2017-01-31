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
	template<typename Game_Handler>
	class world
	{
		Game_Handler& m_gamehandler;
		background_manager<Game_Handler>& m_backgroundmgr;
		data_store_type& m_datastore;

		entity_handler m_entity_handler;

	public:
		world(Game_Handler& gh) :
			m_gamehandler{gh},
			m_backgroundmgr{gh.backgroundmgr()},
			m_entity_handler{gh.rendermgr()},
			m_datastore{gh.datastore()}
		{ }

		void c_player()
		{
			auto plr{factory::create<player>(vec2f{100.f, 600.f})};
			m_entity_handler.create_entity(plr);
		}

		void update(dur duration)
		{
			m_entity_handler.update(duration);
		}

		void render()
		{
			m_entity_handler.render();
		}

		template<bool load_editor>
		void load_level(const entity_proto_vec& entities)
		{
			m_entity_handler.clear();

			for(auto& entity : entities)
			{
				vec2f pos{entity[x], entity[y]};
				auto entity_figure{static_cast<char>(entity[figure])};

				// laod objects
				// rectangle
				if(entity_figure == entity_figure::f_rectangle)
				{
					// we need other entity type when editing
					if constexpr(load_editor)
					{
						auto a{this->create_entity<editor_entity>(pos)};
						a->set_texture(&m_datastore.template get<sf::Texture>
									   ("editor_item_rect.png"));
					}
					else
					{
						auto a{this->create_entity<platform>(pos, vec2f{48.f, 48.f})};
						a->render_object().setTexture(&m_datastore.template get<sf::Texture>
													  ("editor_item_rect.png"));
					}
				}

				// triangles
				else if(entity_figure == entity_figure::f_triangle)
				{
					// we need other entity type when editing
					if constexpr(load_editor)
					{
						auto a{this->create_entity<editor_entity>(pos)};
						a->set_texture(&m_datastore.template get<sf::Texture>
									   ("editor_item_triangle.png"));
					}
					else
					{
						auto a{this->create_entity<platform>(pos, vec2f{48.f, 48.f})};
						a->render_object().setTexture(&m_datastore.template get<sf::Texture>
													  ("editor_item_triangle.png"));
						a->set_propertie(entity_propertie::death);
					}
				}
			}
		}

		template<typename Entity_Type, typename... Args>
		entity_ptr<Entity_Type> create_entity(Args&&... args)
		{
			auto ptr{factory::create<Entity_Type>(std::forward<Args>(args)...)};
			m_entity_handler.create_entity(ptr);
			return ptr;
		}

		std::size_t num_entities() const noexcept
		{return m_entity_handler.num_entities();}

		entity_handler& entityhandler() noexcept
		{return m_entity_handler;}

	private:

	};
}


#endif // RJ_GAME_WORLD_HPP
