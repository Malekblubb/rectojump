//
// Copyright (c) 2013-2017 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_CORE_GAME_GAME_HPP
#define RJ_CORE_GAME_GAME_HPP

#include "game_updater.hpp"
#include "game_window.hpp"
#include <rectojump/game/debug_info.hpp>
#include <rectojump/game/world.hpp>
#include <rectojump/global/common.hpp>
#include <rectojump/shared/level_manager/level_manager.hpp>

namespace rj
{
	template <typename Game_Handler>
	class game
	{
		Game_Handler& m_gamehandler;
		level_manager* m_lvmgr{nullptr};
		rndr& m_render;

		// gameworld
		world<Game_Handler> m_world;

		sf::Text m_warmup_text;
		mlk::hrs_time_pnt m_warmup_start;

	public:
		game(Game_Handler& gh)
			: m_gamehandler{gh},
			  m_render{gh.rendermgr()},
			  m_world{gh},
			  m_warmup_text{"Prepare...", gh.datastore().template get<sf::Font>(
											  settings::text_font())}
		{
			m_warmup_text.setOrigin(
				vec2f{m_warmup_text.getGlobalBounds().width,
					  m_warmup_text.getGlobalBounds().height} /
				2.f);
			auto wsize{settings::get_window_size<vec2f>()};
			m_warmup_text.setPosition({wsize.x / 2.f, 20.f});
		}

		void on_game_start()
		{
			m_warmup_start = mlk::tm::time_pnt();
			//
		}

		void update(dur duration)
		{
			if(m_gamehandler.gamestate() == game_state::running)
				m_world.update(duration);

			if(m_gamehandler.gamestate() == game_state::pre_running) {
				auto remaining_time{
					settings::game_warmup_time() -
					mlk::tm::duration_to_now_as<float>(m_warmup_start)};
				m_warmup_text.setString(std::string{"Prepare...\n"} +
										std::to_string(remaining_time));

				if(remaining_time < 0.f) {
					m_gamehandler.set_gamestate(game_state::running);
				}
			}
		}

		void render()
		{
			m_world.render();
			m_render(m_warmup_text);
		}

		void load_level(const level& lv)
		{
			if(m_lvmgr == nullptr) {
				mlk::lerr(errors::cl_nullptr_access)["rj::game"];
				return;
			}

			m_world.template load_level<false>(lv.entities);
		}

		void set_levelmgr(level_manager* lm) { m_lvmgr = lm; }

		Game_Handler& gamehandler() noexcept { return m_gamehandler; }

		auto& get_world() { return m_world; }
	};
}

#endif// RJ_CORE_GAME_GAME_HPP
