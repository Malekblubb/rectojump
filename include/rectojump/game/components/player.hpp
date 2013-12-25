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
	class player : public entity_rect
	{
		// props
		static constexpr float m_gravity{0.01f};
		static constexpr float m_jump_velo{-1.7f};
		sf::Vector2f m_start_pos{100.f, 500.f};

		// jumping
		bool m_need_jump{false};
		bool m_jumping{false};

		// rotate
		static constexpr float m_max_deg{360.f};
		static constexpr float m_step_deg{-m_jump_velo / m_gravity};

	protected:
		void update(dur duration) override
		{this->try_jump(duration);}

		// 'bind' space key (can't do this in ctor)
		void init() override
		{on_key_pressed(key::Space) += [this]{m_need_jump = true;};}

	public:
		player(const sf::Vector2f& start_pos) :
			entity_rect{start_pos, {40.f, 40.f}, {0.f, 0.f}},
			m_start_pos{start_pos}
		{m_render_object.setOrigin(20.f, 20.f);}


	private:
		// jumping
		void try_jump(dur duration) noexcept
		{
			if(m_need_jump && !m_jumping)
			{
				m_velocity.y = m_jump_velo;
				m_jumping = true;
				m_need_jump = false;
			}

			if(m_jumping)
			{
				m_render_object.move(m_velocity * duration);
				m_velocity.y += m_gravity;

				// rotate step
				m_render_object.rotate(m_step_deg * duration);

				if(m_render_object.getPosition().y >= m_start_pos.y)
				{
					m_jumping = false;
					m_render_object.setPosition(m_start_pos);
				}
			}
		}
	};
}


#endif // RJ_GAME_COMPONENTS_PLAYER_HPP
