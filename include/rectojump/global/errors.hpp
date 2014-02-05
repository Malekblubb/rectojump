//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GLOBAL_ERRORS_HPP
#define RJ_GLOBAL_ERRORS_HPP


namespace rj
{
	enum class errors : char
	{
		// io
		io_open_file,
		io_create_file,
		io_load_sound,

		// network
		ntw_connect,
		ntw_send,
		ntw_recv,

		// level
		lv_load,
		lv_save,
		lv_bad_name,

		// client / other
		cl_nullptr_access,
		cl_data,
	};
}


#endif // RJ_GLOBAL_ERRORS_HPP
