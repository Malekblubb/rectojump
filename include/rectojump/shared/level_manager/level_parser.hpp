//
// Copyright (c) 2013-2017 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_SHARED_LEVEL_MANAGER_LEVEL_PARSER_HPP
#define RJ_SHARED_LEVEL_MANAGER_LEVEL_PARSER_HPP


#include "level_background.hpp"
#include <rectojump/global/common.hpp>

#include <mlk/tools/stl_string_utl.h>
#include <mlk/types/types.h>

#include <string>
#include <vector>


namespace rj
{
	class level_parser
	{
		static constexpr std::uint8_t m_bgdata_id_size{4};
		static constexpr std::uint8_t m_leveldata_id_size{7};
		const mlk::data_packet& m_bg_data;
		const mlk::data_packet& m_level_data;
		std::string m_bgdata_work_string;
		std::string m_leveldata_work_string;

		level_background m_bg_result;
		entity_proto_vec m_level_result;

		bool m_valid{false};

	public:
		level_parser(const mlk::data_packet& bg_data, const mlk::data_packet& level_data) :
			m_bg_data{bg_data},
			m_level_data{level_data}
		{this->parse();}

		const level_background& get_bg_result() const noexcept
		{return m_bg_result;}

		const entity_proto_vec& get_level_result() const noexcept
		{return m_level_result;}

	private:
		void parse()
		{
			this->check_valid();
			if(!m_valid)
				return;

			// prepare the strings
			this->prepare_string();

			// parse background data
			this->parse_bgdata();

			// parse level data
			this->parse_leveldata();
		}

		void parse_bgdata()
		{
			auto nullpos{m_bgdata_work_string.find('\0')};
			auto nullpos2{m_bgdata_work_string.find('\0', nullpos + 1)};
			if(nullpos == std::string::npos || nullpos2 == std::string::npos)
				return;

			m_bg_result =
			{
				m_bgdata_work_string.substr(m_bgdata_id_size, nullpos - m_bgdata_id_size),
				m_bgdata_work_string.substr(nullpos + 1, nullpos2 - nullpos - 1),
				static_cast<std::size_t>(m_bgdata_work_string.substr(nullpos2 + 1,
										m_bgdata_work_string.size() - nullpos2 - 1).at(0))
			};
		}

		void parse_leveldata()
		{
			auto num_ents{mlk::stl_string::count_of("Ent", m_leveldata_work_string)};
			for(std::size_t i{0}; i < num_ents; ++i)
			{
				auto brace_open{m_leveldata_work_string.find('[') + 1};
				auto brace_close{m_leveldata_work_string.find(']')};
				auto line{m_leveldata_work_string.substr(brace_open, brace_close - brace_open)};

				auto space_pos{line.find(' ')};
				entity_prototype tmp_entity;
				for(auto i(0); i < 4; ++i)
				{
					tmp_entity.emplace_back(std::strtof(line.substr(0, space_pos).c_str(),
														nullptr));
					line.erase(0, space_pos + 1);
					space_pos = line.find(' ');
				}

				m_level_result.push_back(tmp_entity);
				m_leveldata_work_string.erase(0, brace_close + 1);
			}
		}

		void check_valid()
		{
			m_valid = m_bg_data.size() > 0 ? (mlk::data_packet{std::begin(m_bg_data),
															   std::begin(m_bg_data) +
															   m_bgdata_id_size} ==
											  header_rj_bg) : false;

			if(m_valid)
			{
				if(m_level_data.size() > 0)
					m_valid = (mlk::data_packet{std::begin(m_level_data),
												std::begin(m_level_data) + m_leveldata_id_size} ==
							   header_rj_level);
				else
					m_valid = false;
			}
		}

		void prepare_string()
		{
			m_bgdata_work_string = {reinterpret_cast<const char*>(m_bg_data.data()),
									m_bg_data.size()};

			m_leveldata_work_string = {reinterpret_cast<const char*>(m_level_data.data()),
									   m_level_data.size()};
			mlk::stl_string::erase_all('{', m_leveldata_work_string);
			mlk::stl_string::erase_all('}', m_leveldata_work_string);
			mlk::stl_string::erase_all(',', m_leveldata_work_string);
		}
	};
}


#endif // RJ_SHARED_LEVEL_MANAGER_LEVEL_PARSER_HPP
