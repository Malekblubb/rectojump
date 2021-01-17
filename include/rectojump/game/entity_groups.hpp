//
// Copyright (c) 2013-2021 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_ENTITY_GROUPS_HPP
#define RJ_GAME_ENTITY_GROUPS_HPP

#include <mlk/tools/bitset.h>

namespace rj
{
	enum class entity_type : char
	{
		player,
		enemy,
		num
	};

	enum entity_propertie : char
	{
		moveable_left,
		jumpable,
		solid,
		death,
		num
	};

	enum entity_figure : char
	{
		f_rectangle,
		f_triangle,
		f_triangles4,
		f_num
	};

	using etypes = mlk::ebitset<entity_type, entity_type::num>;
	using eprops = mlk::ebitset<entity_propertie, entity_propertie::num>;
}

#endif// RJ_GAME_ENTITY_GROUPS_HPP
