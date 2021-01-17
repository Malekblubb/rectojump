
//
// Copyright (c) 2013-2021 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_PARTICLE_MANAGER_HPP
#define RJ_GAME_PARTICLE_MANAGER_HPP

#include "particle.hpp"

namespace rj
{
	template <typename Game_Handler>
	class particle_manager
	{
		Game_Handler& m_gamehandler;
		rndr& m_render;

		std::vector<particle_group> m_particle_groups;

	public:
		particle_manager(Game_Handler& gh)
			: m_gamehandler{gh}, m_render{gh.rendermgr()}
		{
		}

		template <typename... Args>
		void create_particles(Args&&... args)
		{
			m_particle_groups.emplace_back(std::forward<Args>(args)...);
		}

		template <typename Func, typename... Args>
		void create_particles_effect(Func&& f, Args&&... args)
		{
			this->create_particles(args...);
			m_particle_groups.back().on_update_particle = f;
		}

		void update(dur duration)
		{
			for(auto& p : m_particle_groups) p.update(duration);

			mlk::cnt::remove_all_if([](auto& p) { return p.need_destroy(); },
									m_particle_groups);
		}

		void render() { m_render(m_particle_groups); }

		auto num_particles()
		{
			std::size_t result{0};
			for(auto& p : m_particle_groups) result += p.num_particles();
			return result;
		}
	};
}// namespace rj

#endif// RJ_GAME_PARTICLE_MANAGER_HPP
