//
// Copyright (c) 2013-2014 Christoph Malek
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
		// props
		static constexpr float m_gravity{0.01f};
		static constexpr float m_jump_velo{-1.7f};
		static constexpr float m_width{40.f}, m_height{40.f};
		const vec2f m_start_pos;
		float m_ground;

		// jumping
		bool m_need_jump{false};
		bool m_jumping{false};

		// rotate
		static constexpr float m_max_deg{360.f};
		static constexpr float m_step_deg{(-m_jump_velo / m_gravity) / 2.f};
		float m_rotated{0.f};

	public:
		player(const vec2f& start_pos) :
			entity_rect{start_pos, {m_width, m_height}, {0.f, 0.f}},
			m_start_pos{start_pos},
			m_ground{start_pos.y}
		{ }

		~player() = default;

		// 'bind' space key (can't do this in ctor)
		void init() override
		{on_key_pressed(key::Space) += [this]{m_need_jump = true;};}

		void update(dur duration) override
		{
			m_velocity.y += m_gravity;
			this->try_jump();
			this->try_rotate();
			m_render_object.move(m_velocity);

			if(this->bottom_out() > m_ground)
			{
				this->rotate_end();
				this->jump_end();
			}
		}

		void on_collision(float at) noexcept
		{m_ground = at;}

		void on_collision_end() noexcept
		{m_ground = m_start_pos.y;}

	private:
		bool is_on_ground() const noexcept
		{return this->bottom_out() == m_ground;}

		// jumping
		void try_jump() noexcept
		{
			if(m_need_jump && !m_jumping && this->is_on_ground())
			{
				m_velocity.y = m_jump_velo;
				m_jumping = true;
				m_need_jump = false;
			}
		}

		void jump_end() noexcept
		{
			m_render_object.setPosition(m_start_pos.x, m_ground - m_height / 2);
			m_jumping = false;
			m_velocity = {0.f, 0.f};
		}

		void try_rotate() noexcept
		{
			if(!m_jumping)
				return;
			m_render_object.rotate(m_step_deg);
			m_rotated += m_step_deg;
		}

		void rotate_end() noexcept
		{
			if(!m_jumping)
				return;
			// rotate back the rotated way in one step
			m_render_object.rotate(-m_rotated);
			m_rotated = 0.f;
		}
	};
}


#endif // RJ_GAME_COMPONENTS_PLAYER_HPP
