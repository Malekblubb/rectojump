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
	public:
		bool m_need_jump{false};


	protected:
		void update(dur duration) override
		{
			if(m_need_jump)
				this->jump(duration);
		}

		// 'bind' space key (can't do this in ctor)
		void init() override
		{on_key_pressed(key::Space) += [this]{m_need_jump = true;};}

	public:
		player(const sf::Vector2f& pos, const sf::Vector2f& size = {20.f, 20.f}) :
			entity_rect{pos, size}
		{ }


	private:
		void jump(dur duration) noexcept
		{
			std::cout << "jump" << std::endl;
			m_need_jump = false;
		}
	};
}


#endif // RJ_GAME_COMPONENTS_PLAYER_HPP
