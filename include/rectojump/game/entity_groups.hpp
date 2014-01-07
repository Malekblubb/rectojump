//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_ENTITY_GROUPS_HPP
#define RJ_GAME_ENTITY_GROUPS_HPP


#include <mlk/tools/bitset.h>


namespace rj
{
	enum class entity_type : char
	{player, enemy};

	enum class entity_propertie : char
	{moveable_left, jumpable, solid, death};

	enum class entity_figure : char
	{rectagle, triangle};

	using etypes = mlk::bitset<entity_type, 10>;
	using eprops = mlk::bitset<entity_propertie, 10>;
}


#endif // RJ_GAME_ENTITY_GROUPS_HPP
