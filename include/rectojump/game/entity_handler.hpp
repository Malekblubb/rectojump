//
// Copyright (c) 2013 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_ENTITY_HANDLER_HPP
#define RJ_GAME_ENTITY_HANDLER_HPP


#include "entity.hpp"

#include <mlk/log/log.h>

#include <memory>
#include <vector>


namespace rj
{
	template<typename T>
	using entity_ptr = std::shared_ptr<T>;
	using entity_base_ptr = entity_ptr<entity_base>;

	class game;
	class entity_handler
	{
		game& m_game;

		std::vector<entity_base_ptr> m_entities;
		std::size_t m_max_entities;
		std::size_t m_current_id{0};

	public:
		entity_handler(game& g, std::size_t max_entities = 100) :
			m_game{g},
			m_max_entities{max_entities}
		{ }

		bool create_entity(const entity_base_ptr& e) noexcept
		{
			if(this->is_full())
			{
				mlk::lout("rj::entity_handler") << "max_entites limit is reached, can't add more entities";
				return false;
			}
			if(e->is_registered())
			{
				mlk::lout("rj::entity_handler") << "entity with id '" << e->m_id << "' exists already in entity handler, ignoring";
				return false;
			}

			// add/create entity in handler
			this->create_entity_impl(e);
			return true;
		}

		void update(dur duration)
		{
			for(auto& a : m_entities)
				a->update(duration);
		}

		void render()
		{
			for(auto& a : m_entities)
				a->render();
		}

		std::size_t num_entities() const noexcept
		{return m_entities.size();}

	private:
		bool is_full() const noexcept
		{return this->num_entities() >= m_max_entities;}

		void create_entity_impl(const entity_base_ptr& e) noexcept
		{
			// important: set game and init
			e->handler_register(&m_game, m_current_id);
			e->init();
			m_entities.push_back(e);
			++m_current_id;
		}
	};
}


#endif // RJ_GAME_ENTITY_HANDLER_HPP
