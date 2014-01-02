//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GLOBAL_CONFIG_PARSER_HPP
#define RJ_GLOBAL_CONFIG_PARSER_HPP


#include <string>
#include <utility>
#include <vector>


namespace rj
{
	using config_entry = std::pair<std::string, std::string>;
	using config_entry_vec = std::vector<config_entry>;

	class config_parser
	{
		const std::vector<std::string>& m_content;
		config_entry_vec m_entrys;

	public:
		config_parser(const std::vector<std::string>& content) :
			m_content{content}
		{this->parse();}

		auto get_entrys() const noexcept
		-> const decltype(m_entrys)&
		{return m_entrys;}

		bool empty() const noexcept
		{return m_entrys.empty();}

	private:
		void parse() noexcept
		{
			for(auto& a : m_content)
			{
				auto pos_equ(a.find('='));
				if(pos_equ != std::string::npos)
				{
					auto pair(std::make_pair(a.substr(0, pos_equ), a.substr(pos_equ + 1, a.size() - pos_equ - 1)));
					if(!pair.first.empty() && !pair.second.empty())
						m_entrys.push_back(pair);
				}
			}
		}
	};
}


#endif // RJ_GLOBAL_CONFIG_PARSER_HPP
