//
// Copyright (c) 2013-2017 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_PARTICLE_HPP
#define RJ_GAME_PARTICLE_HPP


#include <rectojump/global/common.hpp>
#include <rectojump/global/config_settings.hpp>
#include <mlk/time/simple_timer.h>


namespace rj
{
    class particle
    {
    public:
//        particle() = delete;
//        particle(mlk::ullong interval) :
//            timer{interval}
//        { }
        vec2f velo;
        mlk::tm::simple_timer timer{1000u};
    };

    class particle_group : public sf::Drawable
    {
        std::vector<particle> m_particles;
        sf::VertexArray m_verts;

        vec2f m_start_pos;
        mlk::ullong m_interval;
        sf::Color m_color;
        bool m_exec_once;
        bool m_need_destroy{false};

    public:
        particle_group(std::size_t num_particles = 100, const vec2f& position = {},
                       mlk::ullong interval = 1000u, bool exec_once = true,
                       const sf::Color& particle_color = settings::get_color_light()) :
            m_particles{num_particles},
            m_verts{sf::Points, num_particles},
            m_start_pos{position},
            m_interval{interval},
            m_exec_once{exec_once},
            m_color{particle_color}
        {this->init_particles();}

        void update(dur duration)
        {
            if(m_need_destroy)
                return;

            for(auto i{0}; i < m_particles.size(); ++i)
            {
                m_verts[i].position += m_particles[i].velo;
                if(m_particles[i].timer.timed_out() && !m_exec_once)
                {
                    m_verts[i].position = m_start_pos;
                    m_particles[i].timer.restart();
                }
            }

            if(m_exec_once)
                m_need_destroy = m_particles.back().timer.timed_out();
        }

        auto need_destroy() const noexcept
        {return m_need_destroy;}

        auto num_particles() const noexcept
        {return m_particles.size();}

    private:
        void init_particles()
        {
            for(auto i{0}; i < m_particles.size(); ++i)
            {
                m_particles[i].velo = {mlk::rnd(-0.5f, 0.5f), mlk::rnd(-0.5f, 0.5f)};
                m_particles[i].timer.restart(m_interval);

                m_verts[i].position = m_start_pos;
                m_verts[i].color = m_color;
            }
        }

        void draw(sf::RenderTarget& target, sf::RenderStates states) const
        {
            target.draw(m_verts, states);
        }
    };
}

#endif // RJ_GAME_PARTICLE_HPP
