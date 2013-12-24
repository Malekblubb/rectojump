//
// Copyright (c) 2013 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_WORLD_HPP
#define RJ_GAME_WORLD_HPP


#include "components/platfrom.hpp"
#include "entity_handler.hpp"
#include "components/player.hpp"

namespace rj
{
	class game;
	class world
	{
		entity_handler m_entity_handler;
		float m_move_speed{0.5f};

	public:
		world(game& g) :
			m_entity_handler{g}
		{
//			auto ptr(std::make_shared<platform>(platform{{500.f, 500.f}}));
//			m_entity_handler.create_entity(ptr);
		}

		void do_stuff()
		{
			auto ptr(std::make_shared<player>(player{{600.f, 500.f}}));
//			ptr->set_propertie(entity_properties::moveable_left);
			m_entity_handler.create_entity(ptr);


//			ptr = std::make_shared<platform>(platform{{1500.f, 400.f}});
//			m_entity_handler.create_entity(ptr);

//			auto pl(std::make_shared<player>(player{{500.f, 400.f}}));
//			m_entity_handler.create_entity(pl);
		}

		void update(dur duration)
		{
			m_entity_handler.update(duration);
			// m_background.update(duration);

//			m_entity_handler.update_types(moveable_left);
			//m_entity_handler.update_types(player);
		}

		void render()
		{
			m_entity_handler.render();
		}

	private:

	};
}


#endif // RJ_GAME_WORLD_HPP
