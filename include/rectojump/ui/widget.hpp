//
// Copyright (c) 2013-2021 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_UI_WIDGET_HPP
#define RJ_UI_WIDGET_HPP

#include <rectojump/global/common.hpp>

namespace rj
{
	namespace ui
	{
		class widget : public sf::Drawable
		{
		public:
			virtual void update(dur) {}

			virtual void render() {}
		};
	}
}

#endif// RJ_UI_WIDGET_HPP
