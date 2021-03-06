//
// Copyright (c) 2013-2021 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_DEBUG_INFO_HPP
#define RJ_GAME_DEBUG_INFO_HPP

#include <rectojump/core/render.hpp>
#include <rectojump/global/common.hpp>
#include <rectojump/shared/debug_text.hpp>

#include <mlk/tools/compiletime.h>
#include <mlk/tools/stl_string_utl.h>

namespace rj
{
	struct debug_info_string_base
	{
		virtual ~debug_info_string_base() = default;
		virtual std::string get_formated() const = 0;
	};

	template <typename Func_Ret_Type>
	class debug_info_string : public debug_info_string_base
	{
		const std::string m_unformated_str;
		std::function<Func_Ret_Type()> m_func;

	public:
		template <typename Func>
		debug_info_string(const std::string& unformated_str, Func&& f)
			: m_unformated_str{unformated_str}, m_func{f}
		{
		}

		~debug_info_string() = default;

		std::string get_formated() const override
		{
			auto result{m_unformated_str};
			std::string str;
			if constexpr(std::is_same<std::string, Func_Ret_Type>())
				str = m_func();
			else
				str = std::to_string(m_func());
			auto insert_pos{result.find("%%")};
			if(insert_pos != std::string::npos)
			{
				result.insert(insert_pos + 2, str);
				result.erase(insert_pos, 2);
			}
			return result;
		}
	};

	template <>
	inline std::string debug_info_string<void>::get_formated() const
	{
		return m_unformated_str;
	}

	// debug_info
	template <typename Game_Handler, typename Game>
	class debug_info
	{
		Game_Handler& m_gamehandler;
		rndr& m_render;
		Game& m_game;

		sf::RectangleShape m_background{{100.f, 100.f}};
		debug_text m_text;
		std::vector<mlk::sptr<debug_info_string_base>> m_lines;

	public:
		debug_info(Game_Handler& gh)
			: m_gamehandler{gh},
			  m_render{gh.rendermgr()},
			  m_game{gh.get_game()},
			  m_text{gh.datamgr()}
		{
			this->init();
		}

		void update(dur)
		{
			m_background.setSize(m_text.get_size());
			std::string display_text;
			for(auto& a : m_lines) display_text += a->get_formated();
			m_text.set_text(display_text);
		}

		void render() { m_render(m_background, m_text); }

	private:
		void init() noexcept
		{
			m_background.setPosition({1.f, 1.f});
			m_background.setFillColor({255u, 150u, 123u, 200u});
			m_background.setOutlineThickness(1.f);
			m_background.setOutlineColor({255u, 0u, 0u});

			// Performance
			this->add_title_line("Performance");
			this->add_line("FPS:             %%", [this] {
				return m_gamehandler.gamewindow().get_updater().get_fps();
			});
			this->add_line("FDur:            %%\n", [this] {
				return m_gamehandler.gamewindow()
					.get_updater()
					.get_frameduration();
			});

			// Components
			this->add_title_line("Components");
			this->add_line("Gameworld:       %%", [this] {
				return m_game.get_world().num_entities();
			});
			this->add_line("Background:      %%", [this] {
				return m_gamehandler.backgroundmgr()
					.num_components_current_state();
			});
			this->add_line("Popups:          %%", [this] {
				return m_gamehandler.popupmgr().num_popups();
			});
			this->add_line("Particles:       %%\n", [this] {
				return m_gamehandler.particlemgr().num_particles();
			});

			// Render
			this->add_title_line("Render");
			this->add_line("Ents. on screen: %%\n", [this] {
				return m_gamehandler.get_game()
					.get_world()
					.entityhandler()
					.num_current_rendering();
			});

			// Managers
			this->add_title_line("Managers");
			this->add_line("Data:            %%", [this] {
				return m_gamehandler.datamgr().num_data();
			});
			this->add_line("Levels:          %%\n", [this] {
				return m_gamehandler.levelmgr().num_levels();
			});

			// States
			this->add_title_line("States");
			this->add_line("Active (render): %%", [this] {
				return std::string{state_as_string[int(
					m_gamehandler.current_renderable_state())]};
			});
			this->add_line("Active (all):    %%", [this] {
				std::string active_states;
				for(std::size_t i{0}; i < std::size_t(state::num); ++i)
				{
					if(m_gamehandler.states() & rj::state(i))
						active_states += std::string{state_as_string[i]} + " ";
				}
				return active_states;
			});
		}

		void add_title_line(const std::string& text)
		{
			std::string app{'\n'};
			for(auto& a : text) app += "=";
			auto ptr{std::make_shared<debug_info_string<void>>(
				std::string{text + app + "\n"}, [] {})};
			m_lines.emplace_back(ptr);
		}

		template <typename Func>
		void add_line(const std::string& text, Func&& f)
		{
			auto ptr{std::make_shared<debug_info_string<
				typename mlk::lambda_info<Func>::return_type>>(text + '\n', f)};
			m_lines.emplace_back(ptr);
		}
	};
}// namespace rj

#endif// RJ_GAME_DEBUG_INFO_HPP
