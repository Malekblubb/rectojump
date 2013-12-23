//
// Copyright (c) 2013 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_ENTITY_BASE_HPP
#define RJ_GAME_ENTITY_BASE_HPP


#include "entity_groups.hpp"
#include <rectojump/global/common.hpp>


namespace rj
{
	class entity_handler;
	class game;

	class entity_base
	{
		friend class entity_handler;

		int m_id{-1};
		bool m_is_registered{false};
		eprops m_props;
		etypes m_types;

	protected:
		game* m_game;

		void set_game(game* g)
		{m_game = g;}

		virtual void update(dur duration) = 0;
		virtual void render() = 0;

	public:
		entity_base() = default;

		void set_propertie(eprops::type prop) noexcept
		{m_props |= prop;}

		void set_type(etypes::type type) noexcept
		{m_types |= type;}

		bool has_propertie(eprops::type prop) const noexcept
		{return m_props & prop;}

		bool has_type(etypes::type type) const noexcept
		{return m_types & type;}
	};
}


#endif // RJ_GAME_ENTITY_BASE_HPP
