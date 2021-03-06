//
// Copyright (c) 2013-2021 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_ENTITY_HANDLER_HPP
#define RJ_GAME_ENTITY_HANDLER_HPP

#include "collision.hpp"
#include "components/ent_triangles4.hpp"
#include "components/platform.hpp"
#include "components/player.hpp"
#include "components/triangle.hpp"
#include "entity.hpp"
#include "particle_manager.hpp"
#include <rectojump/global/common.hpp>

#include <mlk/containers/container_utl.h>
#include <mlk/log/log.h>
#include <mlk/types/types.h>

#include <vector>

namespace rj
{
	template <typename T>
	using entity_ptr = mlk::sptr<T>;
	using entity_base_ptr = entity_ptr<entity_base>;
	using player_ptr = entity_ptr<player>;

	class entity_handler
	{
		rndr& m_render;
		particle_manager<game_handler>& m_particlemgr;
		game_handler* m_gamehandler{nullptr};

		player_ptr m_player{nullptr};
		std::vector<entity_base_ptr> m_entities;
		std::size_t m_max_entities;
		std::size_t m_current_rendering{0};
		int m_current_id{0};

		state m_current_state;

		static constexpr float m_despawn_zone{0.f};

	public:
		using iterator = std::vector<entity_base_ptr>::iterator;
		using const_iterator = std::vector<entity_base_ptr>::const_iterator;

		entity_handler(rndr& r, particle_manager<game_handler>& pm,
					   std::size_t max_entities = 100000)
			: m_render{r}, m_particlemgr{pm}, m_max_entities{max_entities}
		{
		}

		void set_gamehandler(game_handler* gh) { m_gamehandler = gh; }

		bool create_entity(const entity_base_ptr& e) noexcept
		{
			if(!this->is_entity_valid(e)) return false;

			this->create_entity_impl(e);// add/create entity in handler
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
			if(this->is_player_registered()) m_player->update(duration);

			// update other
			bool collided{false};
			for(auto& a : m_entities)
			{
				a->update(duration);

				// check collision
				if(this->is_player_registered() &&
				   !a->has_type(entity_type::player))
				{
					if(is_colliding(*m_player, *a))
					{
						collided = true;

						if(m_player->bottom_out() - 5 <= a->top_out())
						{
							m_player->on_collision(a->top_out());
							m_player->render_object().setFillColor({0, 255, 0});
						}
						else
						{
							this->try_player_death();
						}

						if(a->has_propertie(entity_propertie::death))
						{
							// player touched death entity
							this->try_player_death();
						}
					}
				}
				if(a->right_out() <= m_despawn_zone || a->m_destroyed)
					a->destroy();
			}
			this->erase_destroyed();

			if(!collided && this->is_player_registered())
				m_player->on_collision_end();
		}

		void render()
		{
			// render player
			if(this->is_player_registered()) m_player->render();

			// reset the counting of current displayed entities
			m_current_rendering = 0;

			// render other
			for(auto& e : m_entities)
			{
				// only render entities that are currently visible
				if(is_ent_on_screen(e) || m_current_state == state::editor)
				{
					e->render();
					++m_current_rendering;
				}
			}
		}

		void clear() noexcept { m_entities.clear(); }

		// deleting ents on next update
		void delete_entities(std::vector<iterator>& iters)
		{
			for(auto& a : iters) (*a)->destroy();
		}

		auto get_entities_at(const vec2f& at,
							 const vec2f& size = {1.f, 1.f}) noexcept
		{
			std::vector<iterator> result;
			sf::FloatRect at_bounds{at, size};
			for(auto iter{std::begin(m_entities)}; iter != std::end(m_entities);
				++iter)
			{
				sf::FloatRect ent_bounds{
					{(*iter)->left_out(), (*iter)->top_out()}, (*iter)->size()};
				if(ent_bounds.intersects(at_bounds)) result.emplace_back(iter);
			}
			return result;
		}

		bool exists_entity_at(const vec2f& at,
							  const vec2f& size = {1.f, 1.f}) noexcept
		{
			return !this->get_entities_at(at, size).empty();
		}

		iterator begin() { return std::begin(m_entities); }

		iterator end() { return std::end(m_entities); }

		auto num_entities() const noexcept
		{
			return m_entities.size() + this->is_player_registered();
		}

		auto num_current_rendering() const noexcept
		{
			return m_current_rendering;
		}

		auto& player() noexcept { return m_player; }

		bool is_player_registered() const noexcept
		{
			return m_player != nullptr;
		}

		void set_outlines_dbg(bool on)
		{
			for(auto& e : m_entities)
			{
				if(e->figure() == entity_figure::f_rectangle)
				{
					auto ptr{std::static_pointer_cast<platform>(e)};
					ptr->activate_outlines(on);
				}
				else if(e->figure() == entity_figure::f_triangle)
				{
					auto ptr{std::static_pointer_cast<triangle>(e)};
					ptr->activate_outlines(on);
				}
				else if(e->figure() == entity_figure::f_triangles4)
				{
					auto ptr{std::static_pointer_cast<ent_triangles4>(e)};
					ptr->activate_outlines(on);
				}
			}
		}

		void set_current_state(state s) { m_current_state = s; }

	private:
		void on_player_death();

		void try_player_death()
		{
			if(!m_player->is_alive()) return;

			// player should die here
			// do effects, game stats etc....
			m_player->render_object().setFillColor({255, 0, 0});

			// on kill
			m_player->on_kill();
			this->on_player_death();
		}

		// checking the entities
		bool is_entity_valid(const entity_base_ptr& e) const noexcept
		{
			if(m_entities.size() >= m_max_entities)
			{
				mlk::lout("rj::entity_handler")
					<< "max_entities limit is reached,"
					   "can't add more entities";
				return false;
			}
			if(e->is_registered())
			{
				mlk::lout("rj::entity_handler")
					<< "entity with id \"" << e->m_id
					<< "\" exists already in entity handler, ignoring";
				return false;
			}
			return true;
		}

		bool is_ent_on_screen(const entity_base_ptr& e)
		{
			return e->left_out() <= settings::get_window_size<vec2f>().x;
		}

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
			e->handler_register(&m_render, m_current_id);
			e->init();
			++m_current_id;
		}

		void erase_destroyed() noexcept
		{
			mlk::cnt::remove_all_if(
				[](const auto& entity) { return entity->m_destroyed; },
				m_entities);
		}
	};
}// namespace rj

#endif// RJ_GAME_ENTITY_HANDLER_HPP
