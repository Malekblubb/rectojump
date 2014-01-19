//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_CORE_RENDER_HPP
#define RJ_CORE_RENDER_HPP


#include <SFML/Graphics.hpp>


namespace rj
{
	namespace rndr
	{
		// render a single object
		template<typename Game, typename Drawable>
		void ro(Game& g, const Drawable& drawable)
		{g.render_object(drawable);}

		// render helpers
		template<typename Game, typename T, bool is_drawable>
		struct ro_helper
		{
			ro_helper(Game& g, const T& obj)
			{for(auto& a : obj) ro(g, a);}
		};

		template<typename Game, typename T>
		struct ro_helper<Game, T, true>
		{
			ro_helper(Game& g, const T& obj)
			{ro(g, obj);}
		};

		// render multiple objects impl
		template<typename Game>
		void rmo_impl(Game& g)
		{ }

		template<typename Game, typename Head, typename... Tail>
		void rmo_impl(Game& g, const Head& head, Tail&&... tail)
		{
			ro_helper<Game, Head, std::is_base_of<sf::Drawable, Head>::value>{g, head};
			rmo_impl(g, std::forward<Tail>(tail)...);
		}

		// render multiple objects interface
		template<typename Game, typename... Args>
		void rmo(Game& g, Args&&... args)
		{rmo_impl(g, std::forward<Args>(args)...);}
	}
}


#endif // RJ_CORE_RENDER_HPP
