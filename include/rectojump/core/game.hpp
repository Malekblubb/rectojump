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

#include <SFML/Audio.hpp>

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

		// music
		mlk::data_packet m_playback_buffer;
		sf::Music m_playback;
		bool m_need_start_music{false};

		// warmup text
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
			m_need_start_music = true;
		}

		void on_game_end() { m_playback.stop(); }

		void update(dur duration)
		{
			if(m_gamehandler.gamestate() == game_state::running) {
				m_world.update(duration);
				if(m_need_start_music) {
					m_need_start_music = false;
					m_playback.setVolume(100.f);
					m_playback.play();
				}
			}

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
			if(m_gamehandler.gamestate() == game_state::pre_running)
				m_render(m_warmup_text);
		}

		void load_level(const level& lv, bool restart)
		{
			if(m_lvmgr == nullptr) {
				mlk::lerr(errors::cl_nullptr_access)["rj::game"];
				return;
			}

			// set music (if available)
			if(!restart && (lv.music.size() > header_music.size())) {
				m_playback_buffer = {std::begin(lv.music) + header_music.size(),
									 std::end(lv.music)};
				m_playback.openFromMemory(m_playback_buffer.data(),
										  m_playback_buffer.size())
					? mlk::lout("rj::game") << "loaded music data ("
											<< m_playback_buffer.size()
											<< " bytes)"
					: mlk::lerr()["rj::game"] << "could not load music data";
			}

			// load level to world
			m_world.template load_level<false>(lv.entities);
		}

		void set_levelmgr(level_manager* lm) { m_lvmgr = lm; }

		Game_Handler& gamehandler() noexcept { return m_gamehandler; }

		auto& get_world() { return m_world; }
	};
}

#endif// RJ_CORE_GAME_GAME_HPP
