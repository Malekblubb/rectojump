//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_CORE_MAIN_MENU_ITEM_HPP
#define RJ_CORE_MAIN_MENU_ITEM_HPP


#include <string>


namespace rj
{
	using item_id = std::string;

	struct item
	{
		item_id id;
		std::size_t index;
		bool is_valid() const noexcept
		{return !id.empty();}
	};

	bool operator<(const item& lhs, const item& rhs)
	{return lhs.index < rhs.index;}
}


#endif // RJ_CORE_MAIN_MENU_ITEM_HPP
