//
// Copyright (c) 2013-2017 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_CORE_GAME_UPDATER_HPP
#define RJ_CORE_GAME_UPDATER_HPP

#include <rectojump/global/common.hpp>

#include <mlk/signals_slots/slot.h>
#include <mlk/time/time.h>

namespace rj
{
	class game_updater
	{
		float m_current_cut{0.f}, m_next_cut{1.f}, m_step{1.f};
		mlk::hrs_time_pnt m_last_tp{mlk::tm::time_pnt()};
		dur m_frame_duration{0.f};

	public:
		mlk::slot<dur> on_update;
		mlk::slot<> on_render;

		game_updater() = default;

		void update()
		{
			m_current_cut += m_frame_duration;
			for(; m_current_cut >= m_next_cut; m_current_cut -= m_next_cut)
				on_update(m_step);
		}

		float get_fps() const noexcept
		{
			return 1.f / (m_frame_duration / 1000.f);
		}

		dur get_frameduration() const noexcept { return m_frame_duration; }

		void render() { on_render(); }

		// timer
		void start_pt() noexcept { m_last_tp = mlk::tm::time_pnt(); }

		void end_pt() noexcept
		{
			m_frame_duration = mlk::tm::duration_to_now_as<dur>(m_last_tp);
		}
	};
}

#endif// RJ_CORE_GAME_UPDATER_HPP
