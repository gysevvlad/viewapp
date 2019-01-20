#pragma once

#include <optional>
#include <functional>
#include <string>

namespace views_service::controls
{
    class ButtonImpl;

    class Button
    {
    public:
        Button();
        Button(Button &&);
        ~Button();

        /**
         * Click handler property
         */
        Button & setClickHandler(std::function<void()> click_handler)
        {
            m_click_handler = click_handler;
            return *this;
        }

        std::function<void()> & getClickHandler()
        {
            return m_click_handler;
        }

        /**
         * Position property
         */
        Button & setPosition(unsigned x_pos, unsigned y_pos)
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
        Button & setSize(unsigned width, unsigned height)
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
        Button & setText(const std::string & text)
        {
            m_text = std::wstring(text.begin(), text.end());
            return *this;
        }

        Button & setText(std::wstring text)
        {
            m_text = text;
            return *this;
        }

        const std::optional<std::wstring> & getText() const
        {
            return m_text;
        }

        /**
         * Implementation
         */
        void setImpl(std::unique_ptr<ButtonImpl> && impl);

        ButtonImpl * getImpl()
        {
            return m_impl.get();
        }

    private:
        std::unique_ptr<ButtonImpl> m_impl;
        std::optional<unsigned> m_width;
        std::optional<unsigned> m_height;
        std::optional<unsigned> m_x_position;
        std::optional<unsigned> m_y_position;
        std::optional<std::wstring> m_text;
        std::function<void()> m_click_handler;
    };
}
