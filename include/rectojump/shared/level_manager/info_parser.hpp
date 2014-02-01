//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_SHARED_LEVEL_MANAGER_INFO_PARSER_HPP
#define RJ_SHARED_LEVEL_MANAGER_INFO_PARSER_HPP


#include "level_info.hpp"

#include <mlk/types/types.h>

#include <string>


namespace rj
{
	class info_parser
	{
		static constexpr std::size_t m_id_size{6};
		const mlk::data_packet& m_data;
		level_info m_result;

	public:
		info_parser(const mlk::data_packet& data) :
			m_data{data}
		{this->parse();}

		const level_info& get_result() const noexcept
		{return m_result;}

	private:
		void parse()
		{
			std::string as_str{reinterpret_cast<const char*>(m_data.data()), m_data.size()};
			auto null_pos(as_str.find('\0'));

			if(null_pos == std::string::npos)
			{
				m_result = {"invalid", "invalid"};
				return;
			}

			m_result =
			{
				as_str.substr(m_id_size, null_pos - m_id_size),
				as_str.substr(null_pos + 1, as_str.size() - null_pos - 2) // -2: cut space at the end
			};
		}
	};
}


#endif // RJ_SHARED_LEVEL_MANAGER_INFO_PARSER_HPP
