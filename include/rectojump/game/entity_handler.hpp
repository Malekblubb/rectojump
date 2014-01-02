//
// Copyright (c) 2013 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_ENTITY_HANDLER_HPP
#define RJ_GAME_ENTITY_HANDLER_HPP


#include "components/player.hpp"
#include "collision.hpp"
#include "entity.hpp"

#include <mlk/log/log.h>

#include <memory>
#include <vector>


namespace rj
{
	template<typename T>
	using entity_ptr = std::shared_ptr<T>;
	using entity_base_ptr = entity_ptr<entity_base>;
	using player_ptr = entity_ptr<player>;

	class game;
	class entity_handler
	{
		game& m_game;

		player_ptr m_player{nullptr};
		std::vector<entity_base_ptr> m_entities;
		std::size_t m_max_entities;
		std::size_t m_current_id{0};
		bool m_need_check_collision{true};

		static constexpr float m_despawn_zone{0.f};

	public:
		entity_handler(game& g, std::size_t max_entities = 1000) :
			m_game{g},
			m_max_entities{max_entities}
		{ }

		bool create_entity(const entity_base_ptr& e) noexcept
		{
			if(!this->is_entity_valid(e))
				return false;

			this->create_entity_impl(e); // add/create entity in handler
			return true;
		}

		bool create_entity(const player_ptr& p) noexcept
		{
			if(!this->is_entity_valid(p) || this->is_player_registered())
				return false;

			this->create_entity_impl(p);
			return true;
		}

		void update(dur duration)
		{
			// update player
			if(this->is_player_registered())
				m_player->update(duration);

			// update other
			for(auto& a : m_entities)
			{
				a->update(duration);
				if(a->right_out() <= m_despawn_zone)
					a->destroy();
			}

			this->check_collision();

			// erase flagged entities
			this->erase_destroyed();
		}

		void render()
		{
			// render player
			if(this->is_player_registered())
				m_player->render();

			// render other
			for(auto& a : m_entities)
				a->render();
		}

		std::size_t num_entities() const noexcept
		{return m_entities.size() + this->is_player_registered();}

	private:
		void check_collision() noexcept
		{
			if(!this->is_player_registered())
				return;

			bool collided{false};

			for(auto& a : m_entities)
			{
				if(is_colliding(*m_player, *a))
				{
					collided = true;

					if(m_player->bottom_out() - 2 <= a->top_out())
					{
						m_player->on_collision(a->top_out());
						m_player->render_object().setFillColor({0, 255, 0});
					}
					else
						m_player->render_object().setFillColor({255, 0, 0});


					if(a->has_propertie(entity_propertie::death))
					{
						// player touched death entity
						m_player->render_object().setFillColor({255, 0, 0});
					}
				}
			}
			if(!collided)
				m_player->on_collision_end();
		}

		// checking the entities
		bool is_entity_valid(const entity_base_ptr& e) const noexcept
		{
			if(m_entities.size() >= m_max_entities)
			{
				mlk::lout("rj::entity_handler") << "max_entities limit is reached, can't add more entities";
				return false;
			}
			if(e->is_registered())
			{
				mlk::lout("rj::entity_handler") << "entity with id '" << e->m_id << "' exists already in entity handler, ignoring";
				return false;
			}
			return true;
		}

		bool is_player_registered() const noexcept
		{return m_player != nullptr;}

		// create the entities
		void create_entity_impl(const entity_base_ptr& e) noexcept
		{
			this->register_impl(e);
			m_entities.push_back(e);
		}

		void create_entity_impl(const player_ptr& p) noexcept
		{
			this->register_impl(p);
			m_player = p;
		}

		void register_impl(const entity_base_ptr& e) noexcept
		{
			// important: set game and init
			e->handler_register(&m_game, m_current_id);
			e->init();
			++m_current_id;
		}

		void erase_destroyed() noexcept
		{
			m_entities.erase(std::remove_if(std::begin(m_entities), std::end(m_entities),
			[](const entity_base_ptr& entity)
			{return entity->m_destroyed;}), end(m_entities));
		}
	};
}


#endif // RJ_GAME_ENTITY_HANDLER_HPP
