#pragma once

#include <string>
#include <memory>
#include <optional>
#include <list>
#include <variant>

#include "Button.h"
#include "Edit.h"

namespace views_service::controls
{
    class ViewImpl;

    class View
    {
    public:
        View();
        View(View &&);
        ~View();

        /**
         * Create button control
         */
        Button & createButton()
        {
            return std::get<Button>(m_controls.emplace_back(Button()));
        }

        /*
         * Create edit control
         */
        Edit & createEdit()
        {
            return std::get<Edit>(m_controls.emplace_back(Edit()));
        }

        /**
         * Close window
         */
        void Close();

        /**
         * Position property
         */
        View & setPosition(unsigned x_pos, unsigned y_pos)
        {
            m_x_position = x_pos;
            m_y_position = y_pos;
            return *this;
        }

        const std::optional<unsigned> & getXPosition() const
        {
            return m_x_position;
        }

        const std::optional<unsigned> & getYPosition() const
        {
            return m_y_position;
        }

        /**
         * Size property
         */
        View & setSize(unsigned width, unsigned height)
        {
            m_width = width;
            m_height = height;
            return *this;
        }

        const std::optional<unsigned> & getWidth() const
        {
            return m_width;
        }

        const std::optional<unsigned> & getHeight() const
        {
            return m_height;
        }

        /**
         * Text property
         */
        View & setText(const std::string & text)
        {
            m_text = std::wstring(text.begin(), text.end());
            return *this;
        }

        View & setText(std::wstring text)
        {
            m_text = text;
            return *this;
        }

        const std::optional<std::wstring> & getText() const
        {
            return m_text;
        }

        /**
         * Child controls
         */
        auto begin()
        {
            return m_controls.begin();
        }

        auto end()
        {
            return m_controls.end();
        }

        /**
         * Implementation
         */
        void setImpl(std::unique_ptr<ViewImpl> &&);

        ViewImpl* getImpl()
        {
            return m_impl.get();
        }

    private:
        std::unique_ptr<ViewImpl> m_impl;
        std::optional<unsigned> m_width;
        std::optional<unsigned> m_height;
        std::optional<unsigned> m_x_position;
        std::optional<unsigned> m_y_position;
        std::optional<std::wstring> m_text;
        std::list<std::variant<Button, Edit>> m_controls;
    };
}
