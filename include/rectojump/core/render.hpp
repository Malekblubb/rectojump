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

	namespace render
	{
		void render_object(game& g, const sf::Drawable& drawable);
	}
}


#endif // RJ_CORE_RENDER_HPP
