//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_BACKGROUND_BACKGROUND_MANAGER_HPP
#define RJ_GAME_BACKGROUND_BACKGROUND_MANAGER_HPP


#include "background_component.hpp"
#include "basic_background_component.hpp"
#include <rectojump/game/components/gradient_rect.hpp>

#include <mlk/containers/container_utl.h>
#include <mlk/types/types.h>


namespace rj
{
	class game;

	class background_manager
	{
		using bbc_ptr = mlk::sptr<basic_background_component>;
		game& m_game;

		gradient_rect m_bgshape;
		sf::RectangleShape m_textureshape;
		std::vector<bbc_ptr> m_components;

	public:
		background_manager(game& g) :
			m_game{g}
		{ }

		void update(dur duration)
		{
			for(auto& a : m_components)
				a->update(duration);
			this->erase_destroyed();
		}

		void render()
		{
			rndr::ro(m_game, m_bgshape);
			for(auto& a : m_components) a->render();
			rndr::ro(m_game, m_textureshape);
		}

		template<typename Obj_Type, typename... Args>
		mlk::sptr<background_component<Obj_Type>> create_object(Args&&... args)
		{
			auto ptr(std::make_shared<background_component<Obj_Type>>(m_game, std::forward<Args>(args)...));
			m_components.emplace_back(ptr);
			return ptr;
		}

		void set_bg_shape(const gradient_rect& shape) noexcept
		{m_bgshape = shape;}

		void set_tx_shape(const sf::RectangleShape& shape) noexcept
		{m_textureshape = shape;}

		std::size_t num_components() const noexcept
		{return m_components.size();}

	private:
		void erase_destroyed() noexcept
		{
			mlk::cnt::remove_all_if(
			[](const bbc_ptr& p){return p->is_destroyed();}, m_components);
		}
	};
}



#endif // RJ_GAME_BACKGROUND_BACKGROUND_MANAGER_HPP
