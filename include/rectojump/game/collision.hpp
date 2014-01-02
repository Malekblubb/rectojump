//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_COLLISION_HPP
#define RJ_GAME_COLLISION_HPP


namespace rj
{
	template<typename T, typename E>
	bool is_colliding(const T& o1, const E& o2)
	{
		return o1.right_out() >= o2.left_out() &&
				o1.left_out() <= o2.right_out() &&
				o1.bottom_out() >= o2.top_out() &&
				o1.top_out() <= o2.bottom_out();
	}
}


#endif // RJ_GAME_COLLISION_HPP
