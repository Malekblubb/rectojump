
//
// Copyright (c) 2013-2017 Christoph Malek
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
			m_particle_groups.emplace_back(args...);
			bool sec{true};
			m_particle_groups.back().on_update_particle =
				[&sec](auto& particle, auto index, auto size) {
					//					auto
					// rnd{mlk::rnd(-float(index),
					// float(index))};
					if(sec) {
						sec = false;
						//					return;
					}
					//					particle.velo -=
					// vec2f{0.0001f,
					// 0.001f};
					particle.velo -= vec2f{std::cos(index / 100.f) / 100.f,
										   std::sin(index / 100.f) / 100.f} /
									 80.f;
					sec = true;
					//				std::cout << size / (index + 1)
					//<<
					// std::endl;
				};
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
}

#endif// RJ_GAME_PARTICLE_MANAGER_HPP
