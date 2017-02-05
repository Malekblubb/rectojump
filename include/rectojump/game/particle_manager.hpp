
//
// Copyright (c) 2013-2017 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_PARTICLE_MANAGER_HPP
#define RJ_GAME_PARTICLE_MANAGER_HPP


#include "particle.hpp"


namespace rj
{
	template<typename Game_Handler>
	class particle_manager
	{
		Game_Handler& m_gamehandler;
		rndr& m_render;

		std::vector<particle_group> m_particle_groups;

	public:
		particle_manager(Game_Handler& gh) :
			m_gamehandler{gh},
			m_render{gh.rendermgr()}
		{
//			this->create_particles(15000, vec2f{200.f, 200.f}, 2000, true);
		}

		template<typename... Args>
		void create_particles(Args&&... args)
		{
			m_particle_groups.emplace_back(args...);
		}

		void update(dur duration)
		{
			for(auto& p : m_particle_groups)
				p.update(duration);

			mlk::cnt::remove_all_if([](auto& p){return p.need_destroy();}, m_particle_groups);
		}

		void render()
		{
			m_render(m_particle_groups);
		}

		auto num_particles()
		{
			std::size_t result{0};
			for(auto& p : m_particle_groups)
				result += p.num_particles();
			return result;
		}

	private:
	};
}


#endif // RJ_GAME_PARTICLE_MANAGER_HPP
