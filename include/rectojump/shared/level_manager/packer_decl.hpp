//
// Copyright (c) 2013-2017 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_SHARED_LEVEL_MANAGER_PACKER_DECL_HPP
#define RJ_SHARED_LEVEL_MANAGER_PACKER_DECL_HPP

namespace rj
{
	enum class packer_mode : char
	{
		pack,
		unpack
	};

	template <packer_mode mode>
	class level_packer;
}

#endif// RJ_SHARED_LEVEL_MANAGER_PACKER_DECL_HPP
