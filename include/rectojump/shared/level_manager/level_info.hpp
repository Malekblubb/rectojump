//
// Copyright (c) 2013-2017 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_SHARED_LEVEL_MANAGER_LEVEL_INFO_HPP
#define RJ_SHARED_LEVEL_MANAGER_LEVEL_INFO_HPP

#include <mlk/containers/container_utl.h>

#include <string>

namespace rj
{
	struct level_info
	{
		std::string level_name;
		std::string creator_name;
		std::string creation_date;

		int size() const noexcept { return this->data().size(); }

		mlk::data_packet data() const noexcept
		{
			mlk::data_packet result;
			mlk::cnt::append(std::string{"RJINFO\0"}, result);
			mlk::cnt::append(level_name + '\0', result);
			mlk::cnt::append(creator_name + '\0', result);
			mlk::cnt::append(creation_date + '\0', result);
			return result;
		}
	};
}

#endif// RJ_SHARED_LEVEL_MANAGER_LEVEL_INFO_HPP
