//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_UI_LEVEL_WIDGET_ITEM_HPP
#define RJ_UI_LEVEL_WIDGET_ITEM_HPP


#include "widget.hpp"
#include <rectojump/shared/level_manager/level.hpp>


namespace rj
{
	namespace ui
	{
		class level_widget_item : public widget
		{
			sf::RectangleShape m_background{{50, 50}};
			sf::RectangleShape m_preview;
			sf::Text m_level_name;
			sf::Text m_creator_date;
//			sf::Text m_creator_text; // TODO: impl

		public:
			level_widget_item(const level& lv, const sf::Font& font) :
				m_level_name{"", font},
				m_creator_date{"", font}
			{
				if(!lv.is_valid())
					return;

				m_level_name.setString(lv.info.level_name);
				m_creator_date.setString(lv.info.creator_name + "\n" + lv.info.creation_date);
			}

		private:
			void draw(sf::RenderTarget& target, sf::RenderStates states) const override
			{
				target.draw(m_background, states);
				target.draw(m_preview, states);
				target.draw(m_level_name, states);
				target.draw(m_creator_date, states);
			}
		};
	}
}


#endif // RJ_UI_LEVEL_WIDGET_ITEM_HPP
