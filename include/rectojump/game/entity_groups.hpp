//
// Copyright (c) 2013 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_ENTITY_GROUPS_HPP
#define RJ_GAME_ENTITY_GROUPS_HPP


#include <mlk/tools/bitset.h>


namespace rj
{
	enum class entity_types : char
	{player, enemy};

	enum class entity_properties : char
	{moveable, solid, death};

	using etypes = mlk::bitset<entity_types, 10>;
	using eprops = mlk::bitset<entity_properties, 10>;
}


#endif // RJ_GAME_ENTITY_GROUPS_HPP
