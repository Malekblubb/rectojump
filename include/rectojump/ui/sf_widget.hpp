//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_UI_SF_WIDGET_HPP
#define RJ_UI_SF_WIDGET_HPP


#include "widget.hpp"


namespace rj
{
	template<typename Sf_Type>
	class sf_widget : public ui::widget
	{
		Sf_Type m_type;

	public:
		template<typename... Args>
		sf_widget(Args&&... args) :
			m_type{std::forward<Args>(args)...}
		{ }

		auto& operator()() noexcept
		{return m_type;}

		auto& get() noexcept
		{return m_type;}

	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override
		{
			target.draw(m_type, states);
		}
	};

	namespace widget
	{
		using rectangle_shape = sf_widget<sf::RectangleShape>;
		using text = sf_widget<sf::Text>;
	}
}


#endif // RJ_UI_SF_WIDGET_HPP

