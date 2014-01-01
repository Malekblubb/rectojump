//
// Copyright (c) 2013 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_COMPONENTS_PLAYER_HPP
#define RJ_GAME_COMPONENTS_PLAYER_HPP


#include <rectojump/game/entity_rect.hpp>
#include <iostream>


namespace rj
{
	class player final : public entity_rect
	{
		friend class entity_handler;

		// props
		static constexpr float m_gravity{0.01f};
		static constexpr float m_jump_velo{-1.7f};
		static constexpr float m_width{40.f}, m_height{40.f};
		const vec2f m_start_pos{100.f, 500.f};
		float m_ground{500.f};

		// jumping
		bool m_need_jump{false};
		bool m_jumping{false};

		// rotate
		static constexpr float m_max_deg{360.f};
		static constexpr float m_step_deg{-m_jump_velo / m_gravity};

	public:
		player(const vec2f& start_pos) :
			entity_rect{start_pos, {m_width, m_height}, {0.f, 0.f}},
			m_start_pos{start_pos}
		{ }

		~player() = default;

	private:
		// 'bind' space key (can't do this in ctor)
		void init() override
		{on_key_pressed(key::Space) += [this]{m_need_jump = true;};}

		void update(dur duration) override
		{
			m_velocity.y += m_gravity;
			this->try_jump();
			m_render_object.move(m_velocity);

			if(this->bottom_out() > m_ground)
			{
				m_render_object.setPosition(m_start_pos.x, m_ground - m_height / 2);
				m_jumping = false;
				m_velocity = {0.f, 0.f};
			}
		}

		// jumping
		void try_jump() noexcept
		{
			if(m_need_jump && !m_jumping)
			{
				m_velocity.y = m_jump_velo;
				m_jumping = true;
				m_need_jump = false;
			}
		}

		void on_collision(float at) noexcept
		{m_ground = at;}

		void on_collision_end() noexcept
		{m_ground = m_start_pos.y;}
	};
}


#endif // RJ_GAME_COMPONENTS_PLAYER_HPP
