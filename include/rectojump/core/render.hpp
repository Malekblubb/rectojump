//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_CORE_RENDER_HPP
#define RJ_CORE_RENDER_HPP


#include <SFML/Graphics.hpp>


namespace rj
{
	class game;

	namespace rndr
	{
		template<typename Game>
		void ro(Game& g, const sf::Drawable& drawable)
		{g.render_object(drawable);}
	}
}


#endif // RJ_CORE_RENDER_HPP
