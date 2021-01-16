#ifndef RJ_GAME_HUD_HPP
#define RJ_GAME_HUD_HPP

#include <rectojump/global/common.hpp>

#include <string>

namespace rj
{
	enum class hud_text_type
	{
		tries,
		jumps,
		coins
	};

	template <typename Game_Handler>
	class hud
	{
		Game_Handler& m_gamehandler;
		rndr& m_render;

		std::size_t m_num_tries{0};
		std::size_t m_num_jumps{0};

		sf::Text m_text_tries;
		sf::Text m_text_jumps;

		const std::string m_tries_str{"Tries: "};
		const std::string m_jumps_str{"Jumps: "};

	public:
		hud(Game_Handler& gh) : m_gamehandler{gh}, m_render(gh.rendermgr())
		{
			sf::Font& font{m_gamehandler.datastore().template get<sf::Font>(
				"Ubuntu-R.ttf")};

			m_text_tries.setPosition(10, 10);
			m_text_tries.setFont(font);
			m_text_tries.setString("Tries: 0");

			m_text_jumps.setPosition(200, 10);
			m_text_jumps.setFont(font);
			m_text_jumps.setString("Jumps: 0");
		}

		void add_try()
		{
			++m_num_tries;
			update_num(hud_text_type::tries);
		}

		void add_jump()
		{
			++m_num_jumps;
			update_num(hud_text_type::jumps);
		}

		void reset_tries()
		{
			m_num_tries = 0;
			update_num(hud_text_type::tries);
		}
		void reset_jumps()
		{
			m_num_jumps = 0;
			update_num(hud_text_type::jumps);
		}

		void update(dur) {}

		void render() { m_render(m_text_tries, m_text_jumps); }

	private:
		void update_num(hud_text_type type)
		{
			std::string str;
			std::size_t num{0};
			sf::Text* txt = nullptr;

			switch(type)
			{
				case hud_text_type::tries:
					str = m_tries_str;
					num = m_num_tries;
					txt = &m_text_tries;
					break;
				case hud_text_type::jumps:
					str = m_jumps_str;
					num = m_num_jumps;
					txt = &m_text_jumps;
					break;
				case hud_text_type::coins:
					break;
			}
			if(txt != nullptr) txt->setString(str + std::to_string(num));
		}
	};
}// namespace rj

#endif// RJ_GAME_HUD_HPP
