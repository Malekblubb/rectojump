//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_SHARED_LEVEL_MANAGER_PACKER_UNPACK_HPP
#define RJ_SHARED_LEVEL_MANAGER_PACKER_UNPACK_HPP


#include "info_parser.hpp"
#include "level.hpp"
#include "level_parser.hpp"
#include "packer_decl.hpp"
#include <rectojump/global/common.hpp>

#include <mlk/compression/compression.h>
#include <mlk/containers/container_utl.h>
#include <mlk/types/types.h>


namespace rj
{
	template<>
	class level_packer<packer_mode::unpack>
	{
		const mlk::data_packet& m_read_data;
		mlk::data_packet m_work_data;
		level m_result;

		bool m_valid{false};

	public:
		level_packer(const mlk::data_packet& data) :
			m_read_data{data}
		{this->create_level();}

		const level& get_level() const noexcept
		{return m_result;}

	private:
		void create_level()
		{
			this->unpack();
			if(!m_valid || m_work_data.size() <= 0)
				return;
			this->parse();
		}

		void unpack()
		{
			auto unpacked_size(mlk::cnt::make_int(0, m_read_data));
			mlk::cmprs::compressor<mlk::cmprs::cmprs_mode::zlib> cmp{mlk::data_packet{m_read_data.begin() + 4, m_read_data.end()}};
			cmp.unpack(unpacked_size);
			if(!cmp.is_valid())
				return;
			m_work_data = cmp.get();
			m_valid = true;
		}

		void parse()
		{
			auto music_data(mlk::data_packet{std::begin(m_work_data) + sizeof(level_header),
											 std::begin(m_work_data) + sizeof(level_header) + this->music_size()});
			auto level_data(mlk::data_packet{std::begin(m_work_data) + sizeof(level_header) + this->music_size(),
											 std::begin(m_work_data) + sizeof(level_header) + this->music_size() + this->data_size()});
			auto infos_data(mlk::data_packet{std::begin(m_work_data) + sizeof(level_header) + this->music_size() + this->data_size(),
											 std::begin(m_work_data) + sizeof(level_header) + this->music_size() + this->data_size() + this->info_size()});

			level_parser lv_parser{level_data};
			info_parser inf_parser{infos_data};
			m_result = {std::move(music_data), lv_parser.get_result(), inf_parser.get_result()};
		}

		int music_size() const noexcept
		{return mlk::cnt::make_int(0, m_work_data);}

		int data_size() const noexcept
		{return mlk::cnt::make_int(4, m_work_data);}

		int info_size() const noexcept
		{return mlk::cnt::make_int(8, m_work_data);}
	};
}


#endif // RJ_SHARED_LEVEL_MANAGER_PACKER_UNPACK_HPP
