//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_SHARED_LEVEL_MANAGER_LEVEL_DATA_HPP
#define RJ_SHARED_LEVEL_MANAGER_LEVEL_DATA_HPP


#include <rectojump/game/entity_groups.hpp>
#include <rectojump/global/common.hpp>

#include <mlk/containers/container_utl.h>


namespace rj
{
	class level_data
	{
		std::vector<std::string> m_data{"RJLEVEL"};

	public:
		level_data() = default;

		void add_entity(entity_figure figure, entity_propertie prop, const vec2f& pos) noexcept
		{
			auto str(mlk::stl_string::str_format("Ent[%%, %%, {%%, %%}]", figure, prop, pos.x, pos.y));
			m_data.push_back(str);
		}

		auto num_entities() const noexcept
		-> decltype(m_data.size())
		{return m_data.size() - 1;}

		int size() const noexcept
		{return this->data().size();}

		mlk::data_packet data() const noexcept
		{
			mlk::data_packet result;
			for(auto& a : m_data)
				mlk::cnt::append(a + '\0', result);
			return result;
		}
	};
}


#endif // RJ_SHARED_LEVEL_MANAGER_LEVEL_DATA_HPP