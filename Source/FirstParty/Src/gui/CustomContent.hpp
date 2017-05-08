#pragma once

#ifndef CUSTOM_CONTENT_HPP
#define CUSTOM_CONTENT_HPP

#include "MenuElement.hpp"

#include <functional>
#include <memory>

/// this class represent an element which will be filled with external content
class CustomContent : public MenuElement
{
    std::function<void (int id, const DrawParameter& params)> m_callback;

    std::unique_ptr<MenuElement> onClone() const override
    {
        return std::unique_ptr<MenuElement>(new CustomContent(getId()));
    }

    void onDrawElement(const DrawParameter& params) override
    {
        if(m_callback)
            m_callback(getId(), params);
    }

public:
    CustomContent(int id) :
        MenuElement(id, MenuElementType::CustomContent, ScreenLocation())
    { }

    void registerOnDraw(std::function<void (int id, const DrawParameter& params)> callback)
    {
        m_callback = callback;
    }
};

#endif // CUSTOM_CONTENT_HPP
