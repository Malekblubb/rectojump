//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_SHARED_LEVEL_MANAGER_PACKER_PACK_HPP
#define RJ_SHARED_LEVEL_MANAGER_PACKER_PACK_HPP


#include "level.hpp"
#include "level_data.hpp"
#include "level_info.hpp"
#include "packer_decl.hpp"
#include <rectojump/global/common.hpp>

#include <mlk/compression/compression.h>
#include <mlk/containers/container_utl.h>
#include <mlk/types/types.h>


namespace rj
{
	template<>
	class level_packer<packer_mode::pack>
	{
		const music_data& m_music;
		const level_data& m_data;
		const level_background& m_background;
		const level_info& m_info;

		mlk::data_packet m_packed_level;

		bool m_valid{false};

	public:
		level_packer(const music_data& music, const level_background& bg, const level_data& data, const level_info& info) :
			m_music{music},
			m_data{data},
			m_background{bg},
			m_info{info}
		{this->create_level();}

		bool is_valid() const noexcept
		{return m_valid;}

		auto packed_data() const noexcept
		-> const mlk::data_packet&
		{return m_packed_level;}

	private:
		void create_level()
		{
			this->make_header();
			this->make_content();
			this->compress();
		}

		void make_header()
		{
			level_header header{this->music_size(), this->background_size(), this->data_size(), this->info_size(), 0};
			mlk::cnt::append(std::string{reinterpret_cast<char*>(&header), sizeof header}, m_packed_level);
		}

		void make_content()
		{
			m_packed_level += m_music;
			m_packed_level += m_background.data();
			m_packed_level += m_data.data();
			m_packed_level += m_info.data();
		}

		void compress()
		{
			auto unpacked_size(static_cast<int>(m_packed_level.size()));
			mlk::cmprs::compressor<mlk::cmprs::cmprs_mode::zlib> cmp{m_packed_level};
			cmp.pack();
			if(!cmp.is_valid())
				return;

			m_packed_level = cmp.get();
			mlk::cnt::insert_front(std::string{reinterpret_cast<char*>(&unpacked_size), sizeof unpacked_size},
								   m_packed_level);
			m_valid = true;
		}

		int music_size() const noexcept
		{return m_music.size();}

		int background_size() const noexcept
		{return m_background.size();}

		int data_size() const noexcept
		{return m_data.size();}

		int info_size() const noexcept
		{return m_info.size();}
	};

}


#endif // RJ_SHARED_LEVEL_MANAGER_PACKER_PACK_HPP
