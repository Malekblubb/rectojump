//
// Copyright (c) 2013-2017 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GLOBAL_ERROR_INSERTER_HPP
#define RJ_GLOBAL_ERROR_INSERTER_HPP


#include "errors.hpp"
#include <rectojump/shared/error_handler.hpp>

#include <mlk/containers/containers.h>
#include <mlk/log/log.h>


namespace rj
{
	class error_inserter
	{
		error_handler& m_errorhandler;

		using et = mlk::cnt::error_type<errors>;
		std::vector<et> m_list;

	public:
		error_inserter(error_handler& eh) :
			m_errorhandler{eh}
		{
			this->create_list();
			for(auto& a : m_list)
				mlk::lerr_i().link_error(a);
		}

	private:
		void create_list()
		{
			m_list.emplace_back(errors::io_open_file, "failed to open file.");
			m_list.emplace_back(errors::io_create_file, "failed to create file.");
			m_list.emplace_back(errors::io_load_sound, "failed to load sound.");
			m_list.emplace_back(errors::ntw_connect, "failed to connect.");
			m_list.emplace_back(errors::ntw_send, "failed to send.");
			m_list.emplace_back(errors::ntw_recv, "failed to recv.");
			m_list.emplace_back(errors::cl_nullptr_access, "FATAL: access to odd memory.",
								mlk::slot<>{[this]{m_errorhandler.exec_error_instance(
												   errors::cl_nullptr_access);}});
			m_list.emplace_back(errors::cl_data, "FATAL: needed data not loaded",
								mlk::slot<>{[this]{m_errorhandler.exec_error_instance(
												   errors::cl_data);}});
			m_list.emplace_back(errors::lv_bad_name, "Bad level name");
		}
	};
}


#endif // RJ_GLOBAL_ERROR_INSERTER_HPP
