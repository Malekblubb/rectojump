//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_CORE_RENDER_HPP
#define RJ_CORE_RENDER_HPP


namespace rj
{
	namespace rndr
	{
		template<typename Game, typename Drawable>
		void ro(Game& g, const Drawable& drawable)
		{g.render_object(drawable);}

		template<typename Game>
		void rmo_impl(Game& g)
		{ }

		template<typename Game, typename Head, typename... Tail>
		void rmo_impl(Game& g, const Head& head, Tail&&... tail)
		{
			ro(g, head);
			rmo_impl(g, std::forward<Tail>(tail)...);
		}

		template<typename Game, typename... Args>
		void rmo(Game& g, Args&&... args)
		{rmo_impl(g, std::forward<Args>(args)...);}
	}
}


#endif // RJ_CORE_RENDER_HPP
