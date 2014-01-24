//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_WORLD_HPP
#define RJ_GAME_WORLD_HPP


#include "entity_handler.hpp"
#include "factory.hpp"
#include "components/platfrom.hpp"
#include "components/player.hpp"
#include "components/triangle.hpp"

#include <mlk/tools/random_utl.h>


namespace rj
{
	class world
	{
		entity_handler m_entity_handler;


	public:
		world(rndr& r) :
			m_entity_handler{r}
		{ }

		void c_player()
		{
			auto plr(factory::create<player>(vec2f{100.f, 600.f}));
			m_entity_handler.create_entity(plr);
		}

		void c_world()
		{
//			auto rec1(std::make_shared<platform>(platform{{600.f, 450.f}, {40.f, 40.f}}));
			auto rec1(factory::create<platform>(vec2f{600.f, 450.f}, vec2f{40.f, 40.f}));
			m_entity_handler.create_entity(rec1);

			auto rec2(std::make_shared<platform>(platform{{700.f, 400.f}, {40.f, 40.f}}));
			m_entity_handler.create_entity(rec2);

			auto rec3(std::make_shared<platform>(platform{{800.f, 400.f}, {40.f, 40.f}}));
			m_entity_handler.create_entity(rec3);

			auto rec4(std::make_shared<platform>(platform{{900.f, 350.f}, {40.f, 40.f}}));
			m_entity_handler.create_entity(rec4);

			auto tri(std::make_shared<triangle>(triangle{{1000.f, 500.f}}));
			m_entity_handler.create_entity(tri);
		}

		void c_ent(const vec2f& pos)
		{
			auto rec1(factory::create<platform>(pos, vec2f{40.f, 40.f}));
			m_entity_handler.create_entity(rec1);
		}

		void c_ent_death(const vec2f& pos)
		{
			auto r(mlk::rnd<std::uint8_t>(0, 255)), g(mlk::rnd<std::uint8_t>(0, 255)), b(mlk::rnd<std::uint8_t>(0, 255)), a(mlk::rnd<std::uint8_t>(150, 255));
			auto rec1(factory::create<platform>(pos, vec2f{40.f, 40.f}));
			rec1->render_object().setFillColor({r, g, b, a});
			rec1->set_propertie(entity_propertie::death);
			m_entity_handler.create_entity(rec1);
		}

		void update(dur duration)
		{
			m_entity_handler.update(duration);

			// m_background.update(duration);
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
				auto tmp(factory::create<platform>(vec2f{entity[x], entity[y]}, vec2f{40.f, 40.f}));
				m_entity_handler.create_entity(tmp);
			}
		}


		std::size_t num_entities() const noexcept
		{return m_entity_handler.num_entities();}

	private:

	};
}


#endif // RJ_GAME_WORLD_HPP
