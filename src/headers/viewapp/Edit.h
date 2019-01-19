#pragma once

#include <string>
#include <optional>
#include <memory>

namespace views_service::controls
{
    class EditImpl;

    class Edit
    {
    public:
        Edit();
        Edit(Edit && edit);
        ~Edit();

        /**
         * Position property
         */
        Edit & setPosition(unsigned x_pos, unsigned y_pos) 
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
        Edit & setSize(unsigned width, unsigned height)
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
        Edit & setText(const std::string & text)
        {
            m_text = std::wstring(text.cbegin(), text.cend());
            return *this;
        }

        Edit & setText(std::wstring text)
        {
            m_text = text;
        }

        const std::optional<std::wstring> & getText() const
        {
            return m_text;
        }

        /**
         * Implementation
         */
        void setImpl(std::unique_ptr<EditImpl> && impl);

        EditImpl * getImpl()
        { 
            return m_impl.get(); 
        }

    private:
        std::unique_ptr<EditImpl> m_impl;
        std::optional<unsigned> m_width;
        std::optional<unsigned> m_height;
        std::optional<unsigned> m_x_position;
        std::optional<unsigned> m_y_position;
        std::optional<std::wstring> m_text;
    };
}
