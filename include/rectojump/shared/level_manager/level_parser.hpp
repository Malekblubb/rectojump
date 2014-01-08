//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_SHARED_LEVEL_MANAGER_LEVEL_PARSER_HPP
#define RJ_SHARED_LEVEL_MANAGER_LEVEL_PARSER_HPP


#include <rectojump/global/common.hpp>

#include <mlk/tools/stl_string_utl.h>
#include <mlk/types/types.h>

#include <string>
#include <vector>


namespace rj
{
	class level_parser
	{
		static constexpr std::uint8_t m_id_size{7};
		const mlk::data_packet& m_data;
		std::string m_work_string;

		entity_proto_vec m_result;

		bool m_valid{false};

	public:
		level_parser(const mlk::data_packet& data) :
			m_data{data}
		{this->parse();}

		const entity_proto_vec& get_result() const noexcept
		{return m_result;}

	private:
		void parse()
		{
			this->check_valid();
			if(!m_valid)
				return;

			this->prepare_string();
			auto num_ents(mlk::stl_string::count_of("Ent", m_work_string));
			for(std::size_t i{0}; i < num_ents; ++i)
			{
				auto brace_open(m_work_string.find('[') + 1);
				auto brace_close(m_work_string.find(']'));
				auto line(m_work_string.substr(brace_open, brace_close - brace_open));

				auto space_pos(line.find(' '));
				entity_prototype tmp_entity;
				for(auto i(0); i < 4; ++i)
				{
					tmp_entity.emplace_back(std::strtof(line.substr(0, space_pos).c_str(), nullptr));
					line.erase(0, space_pos + 1);
					space_pos = line.find(' ');
				}

				m_result.push_back(tmp_entity);
				m_work_string.erase(0, brace_close + 1);
			}
		}

		void check_valid()
		{
			m_valid = m_data.size() > 0 ? (mlk::data_packet{std::begin(m_data), std::begin(m_data) + m_id_size} ==
										   mlk::data_packet{'R', 'J', 'L', 'E', 'V', 'E', 'L'}) : false;
		}

		void prepare_string()
		{
			m_work_string = {reinterpret_cast<const char*>(m_data.data()), m_data.size()};
			mlk::stl_string::erase_all('{', m_work_string);
			mlk::stl_string::erase_all('}', m_work_string);
			mlk::stl_string::erase_all(',', m_work_string);
		}
	};
}


#endif // RJ_SHARED_LEVEL_MANAGER_LEVEL_PARSER_HPP
