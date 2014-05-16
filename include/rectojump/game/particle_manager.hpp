 
//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_PARTICLE_MANAGER_HPP
#define RJ_GAME_PARTICLE_MANAGER_HPP




namespace rj
{
	template<typename Game_Handler>
	class particle_manager : public sf::Drawable
	{
		Game_Handler& m_gamehandler;




	public:
		particle_manager(Game_Handler& gh) :
			m_gamehandler{gh}
		{

		}

		void update(dur)
		{


		}

		void render()
		{
			m_gamehandler.rendermgr()(*this);
		}

	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const
		{

		}
	};
}


#endif // RJ_GAME_PARTICLE_MANAGER_HPP
