//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_ENTITY_BASE_HPP
#define RJ_GAME_ENTITY_BASE_HPP


#include "entity_groups.hpp"
#include <rectojump/global/common.hpp>
#include <rectojump/shared/input.hpp>


namespace rj
{
	class entity_handler;
	class game;

	class entity_base
	{
		friend class entity_handler;

		int m_id{-1};
		bool m_is_registered{false};
		bool m_destroyed{false};
		eprops m_props;
		etypes m_types;

		// handler functions
		void handler_register(game* g, int id) noexcept
		{
			m_game = g;
			m_id = id;
			m_is_registered = true;
		}

		void destroy() noexcept
		{m_destroyed = true;}

	protected:
		game* m_game{nullptr};

		virtual void update(dur duration) = 0;
		virtual void render() = 0;
		virtual void init() { } // called by 'entity_handler'

	public:
		entity_base() = default;
		virtual ~entity_base() = default;

		bool is_registered() const noexcept
		{return m_is_registered || m_id != -1;}

		void set_propertie(eprops::type prop) noexcept
		{m_props |= prop;}

		void set_type(etypes::type type) noexcept
		{m_types |= type;}

		bool has_propertie(eprops::type prop) const noexcept
		{return m_props & prop;}

		bool has_type(etypes::type type) const noexcept
		{return m_types & type;}

		// position, collision
		virtual const vec2f& pos() const noexcept = 0;
		virtual float pos_x() const noexcept = 0;
		virtual float pos_y() const noexcept = 0;
		virtual float top_out() const noexcept = 0;
		virtual float bottom_out() const noexcept = 0;
		virtual float left_out() const noexcept = 0;
		virtual float right_out() const noexcept = 0;
	};
}


#endif // RJ_GAME_ENTITY_BASE_HPP
