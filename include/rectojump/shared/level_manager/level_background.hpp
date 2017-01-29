//
// Copyright (c) 2013-2017 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_SHARED_LEVEL_MANAGER_LEVEL_BACKGROUND_HPP
#define RJ_SHARED_LEVEL_MANAGER_LEVEL_BACKGROUND_HPP


#include <rectojump/global/common.hpp>
#include <rectojump/shared/utils.hpp>

#include <mlk/containers/container_utl.h>


namespace rj
{
	class level_background
	{
		sf::Color m_startcolor;
		sf::Color m_endcolor;
		std::size_t m_pointcount;
		mlk::data_packet m_data;

	public:
		level_background() = default;

		level_background(const std::string& start_hex, const std::string& end_hex,
						 std::size_t pointcount) :
			m_startcolor{to_rgb(start_hex)},
			m_endcolor{to_rgb(end_hex)},
			m_pointcount{pointcount}
		{
			mlk::cnt::append(std::string{"RJBG\0"}, m_data);
			mlk::cnt::append(start_hex + '\0', m_data);
			mlk::cnt::append(end_hex + '\0', m_data);
			m_data.emplace_back(pointcount);
		}

		const sf::Color& startcolor() const noexcept
		{return m_startcolor;}

		const sf::Color& endcolor() const noexcept
		{return m_endcolor;}

		std::size_t pointcount() const noexcept
		{return m_pointcount;}

		auto size() const noexcept
		{return m_data.size();}

		const mlk::data_packet& data() const noexcept
		{return m_data;}
	};
}


#endif // RJ_SHARED_LEVEL_MANAGER_LEVEL_BACKGROUND_HPP
