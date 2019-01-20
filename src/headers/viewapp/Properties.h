#pragma once

#include <optional>
#include <string>

namespace views_service::controls
{
    template<class T>
    class Impl;

    template<class T>
    class Base
    {
    public:
        /**
         * Position property
         */
        T & setPosition(unsigned x_pos, unsigned y_pos)
        {
            m_x_position = x_pos;
            m_y_position = y_pos;
            return static_cast<T&>(*this);
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
        T & setSize(unsigned width, unsigned height)
        {
            m_width = width;
            m_height = height;
            return static_cast<T&>(*this);
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
        T & setText(const std::string & text)
        {
            m_text = std::wstring(text.cbegin(), text.cend());
            return static_cast<T&>(*this);
        }

        T & setText(std::wstring text)
        {
            m_text = text;
            return static_cast<T&>(*this);
        }

        const std::optional<std::wstring> & getText() const
        {
            return m_text;
        }

        /**
         * Implementation
         */
        Impl<T> * getImpl()
        {
            return m_impl.get();
        }

        void setImpl(std::unique_ptr<Impl<T>> && impl);

    private:
        std::unique_ptr<Impl<T>> m_impl;
        std::optional<unsigned> m_width;
        std::optional<unsigned> m_height;
        std::optional<unsigned> m_x_position;
        std::optional<unsigned> m_y_position;
        std::optional<std::wstring> m_text;
    };
}