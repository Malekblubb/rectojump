//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_COMPONENTS_STAR5_HPP
#define RJ_GAME_COMPONENTS_STAR5_HPP


#include <rectojump/global/common.hpp>

#include <SFML/Graphics.hpp>

#include <cmath>


namespace rj
{
	class star5 : public sf::ConvexShape
	{
		static constexpr float m_pi{3.14159265359f};
		float m_length{0.f};

	public:
		star5()
		{this->init();}

		star5(float length) :
			m_length{length}
		{this->init();}

		star5(float length, const vec2f& pos) :
			star5{length}
		{this->setPosition(pos);}

		void set_length(float l) noexcept
		{m_length = l; this->recalculate();}

		float get_length() const noexcept
		{return m_length;}

	private:
		void init() noexcept
		{
			this->setPointCount(5);
			this->recalculate();
		}

		void recalculate() noexcept
		{
			this->setPoint(0, {m_length * std::cos((2 * m_pi) / 5), m_length * std::sin((2 * m_pi) /5)});
			this->setPoint(1, {m_length * std::cos((6 * m_pi) / 5), m_length * std::sin((6 * m_pi) / 5)});
			this->setPoint(2, {m_length * std::cos((10 * m_pi) / 5), m_length * std::sin((10 * m_pi) / 5)});
			this->setPoint(3, {m_length * std::cos((4 * m_pi) / 5), m_length * std::sin((4 * m_pi) / 5)});
			this->setPoint(4, {m_length * std::cos((8 * m_pi) / 5), m_length * std::sin((8 * m_pi) / 5)});
		}
	};
}


#endif // RJ_GAME_COMPONENTS_STAR5_HPP
