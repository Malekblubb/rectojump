//
// Copyright (c) 2013-2021 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_EDITOR_BUTTON_ITEM_HPP
#define RJ_GAME_EDITOR_BUTTON_ITEM_HPP

#include <rectojump/game/entity_groups.hpp>
#include <rectojump/global/config_settings.hpp>
#include <rectojump/ui/button.hpp>

namespace rj
{
    class button_item : public ui::button
    {
        entity_figure m_figure;
        const sf::Color m_hover_color{settings::get_color_light()};

    public:
        button_item(const vec2f& size, const vec2f& pos) : button{size, pos}
        {
            this->init_base();
        }

        void update(dur duration) override { button::update(duration); }

        void set_figure(entity_figure f) noexcept { m_figure = f; }

        entity_figure get_figure() const noexcept { return m_figure; }

    private:
        void init() override
        {
            m_shape.setFillColor({0, 0, 0, 0});
            m_shape.setOutlineColor(settings::get_color_default_dark());
            m_shape.setOutlineThickness(2.f);
        }

        void on_hover() override { m_shape.setOutlineColor(m_hover_color); }
    };
}// namespace rj

#endif// RJ_GAME_EDITOR_BUTTON_ITEM_HPP
