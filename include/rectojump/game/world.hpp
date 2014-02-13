//
// Copyright (c) 2013-2014 Christoph Malek
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
#include <rectojump/shared/data_store.hpp>

#include <mlk/tools/random_utl.h>


namespace rj
{
	template<typename Game_Handler>
	class world
	{
		Game_Handler& m_gamehandler;
		background_manager& m_backgroundmgr;
		typename Game_Handler::data_store_type& m_datastore;

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
			auto plr(factory::create<player>(vec2f{100.f, 600.f}));
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

		void load_level(const entity_proto_vec& entities)
		{
			m_entity_handler.clear();

			for(auto& entity : entities)
			{
				mlk::sptr<entity_base> ptr{nullptr};
				vec2f pos{entity[x], entity[y]};

				auto entity_figure(static_cast<char>(entity[figure]));

				if(entity_figure == entity_figure::f_rectangle)
				{
					auto obj(factory::create<platform>(pos, vec2f{48.f, 48.f}));
					obj->render_object().setTexture(&m_datastore.template get<sf::Texture>("editor_item_rect.png"));
//					obj->render_object().setOutlineThickness(1);
//					obj->render_object().setOutlineColor(sf::Color::Red);
					ptr = obj;
				}
				else if(entity_figure == entity_figure::f_triangle)
				{
					auto obj(factory::create<platform>(pos, vec2f{48.f, 48.f}));
					obj->render_object().setTexture(&m_datastore.template get<sf::Texture>("editor_item_triangle.png"));
					obj->set_propertie(entity_propertie::death);
					ptr = obj;
				}


				switch(static_cast<char>(entity[figure]))
				{
				case entity_figure::f_rectangle:
				{
//					auto obj(factory::create<platform>(pos, vec2f{48.f, 48.f}));
//					obj->render_object().setTexture(&m_datastore.template get<sf::Texture>("editor_item_rect.png"));
//					ptr = obj;

					break;
				}
				case entity_figure::f_triangle:
//					ptr = factory::create<triangle>(pos, 19.f);
					break;
				case entity_figure::f_triangles4:
					m_backgroundmgr.create_object<triangles4>(pos, vec2f{50.f, 200.f}, 0, 0.5f, vec2f{0, 0});
					break;
				default:
					ptr = nullptr;
					break;
				}

				if(ptr)
					m_entity_handler.create_entity(ptr);
			}
		}

		template<typename Entity_Type, typename... Args>
		entity_ptr<Entity_Type> create_entity(Args&&... args)
		{
			auto ptr(factory::create<Entity_Type>(std::forward<Args>(args)...));
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
