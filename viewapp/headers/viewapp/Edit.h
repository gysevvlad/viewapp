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
        Edit(Edit && edit) noexcept;
        ~Edit();

        Edit & setPosition(unsigned x_pos, unsigned y_pos);
        const std::optional<unsigned> & getXPosition() const;
        const std::optional<unsigned> & getYPosition() const;

        Edit & setSize(unsigned width, unsigned height);
        const std::optional<unsigned> & getWidth() const;
        const std::optional<unsigned> & getHeight() const;

        Edit & setText(const std::string & text);
        Edit & setText(std::wstring text);
        const std::optional<std::wstring> & getText() const;

        void setImpl(std::unique_ptr<EditImpl> && impl);
        EditImpl * getImpl();

    private:
        std::unique_ptr<EditImpl>   m_impl;
        std::optional<unsigned>     m_width;
        std::optional<unsigned>     m_height;
        std::optional<unsigned>     m_x_position;
        std::optional<unsigned>     m_y_position;
        std::optional<std::wstring> m_text;
    };
}