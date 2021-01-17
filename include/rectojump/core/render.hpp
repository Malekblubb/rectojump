//
// Copyright (c) 2013-2021 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_CORE_RENDER_HPP
#define RJ_CORE_RENDER_HPP

#include <SFML/Graphics.hpp>

namespace rj
{
	namespace render_utl
	{
		// render a single object
		template <typename Game_Handler, typename Drawable>
		void ro(Game_Handler& gh, const Drawable& drawable)
		{
			gh.render_object(drawable);
		}

		// render helpers
		template <typename Game_Handler, typename T, bool is_drawable>
		struct ro_helper
		{
			ro_helper(Game_Handler& gh, const T& obj)
			{
				for(auto& a : obj) ro(gh, a);
			}
		};

		template <typename Game_Handler, typename T, typename E>
		struct ro_helper<Game_Handler, std::map<T, E>, false>
		{
			ro_helper(Game_Handler& gh, const std::map<T, E>& m)
			{
				for(auto& a : m) ro(gh, a.second);
			}
		};

		template <typename Game_Handler, typename T>
		struct ro_helper<Game_Handler, T, true>
		{
			ro_helper(Game_Handler& gh, const T& obj) { ro(gh, obj); }
		};
	}// namespace render_utl

	template <typename Game_Handler>
	class render
	{
		Game_Handler& m_gamehandler;

	public:
		render(Game_Handler& gh) : m_gamehandler{gh} {}

		template <typename... Args>
		void operator()(Args&&... args)
		{
			this->rmo(std::forward<Args>(args)...);
		}

		template <typename... Args>
		void rmo(Args&&... args)
		{
			this->rmo_impl(std::forward<Args>(args)...);
		}

		// render multiple objects impl
		void rmo_impl() {}

		template <typename Head, typename... Tail>
		void rmo_impl(const Head& head, Tail&&... tail)
		{
			render_utl::ro_helper<Game_Handler, Head,
								  std::is_base_of<sf::Drawable, Head>::value>{
				m_gamehandler, head};
			rmo_impl(std::forward<Tail>(tail)...);
		}
	};
}// namespace rj

#endif// RJ_CORE_RENDER_HPP
