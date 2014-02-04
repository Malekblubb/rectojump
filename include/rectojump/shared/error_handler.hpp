//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_SHARED_ERROR_HANDLER_HPP
#define RJ_SHARED_ERROR_HANDLER_HPP


#include <mlk/signals_slots/slot.h>

#include <rectojump/global/common.hpp>
#include <rectojump/global/errors.hpp>


namespace rj
{
	struct error_instance
	{
		sf::Text error_text;
		mlk::slot<> error_event;
	};

	class error_handler
	{
		std::map<errors, error_instance> m_instances;
		errors m_current_error;

	public:
		template<typename Func>
		error_instance& create_error_instance(errors e, const std::string& error_text, const sf::Font& font, Func&& error_event)
		{
			m_instances[e] = {{error_text, font}, {error_event}};
			return m_instances[e];
		}

		void exec_error_instance(errors e)
		{
			if(m_instances.find(e) == std::end(m_instances))
				return;

			m_instances[e].error_event();
			m_current_error = e;
		}

		const error_instance& get_current_error_instance() noexcept
		{return m_instances[m_current_error];}
	};
}



#endif // RJ_SHARED_ERROR_HANDLER_HPP
