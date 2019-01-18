#pragma once

#include <string>
#include <memory>
#include <optional>

namespace views_service::controls
{
    class ViewImpl;

    class View
    {
    public:
        View();
        ~View();

        View & setPosition(unsigned x_pos, unsigned y_pos);
        const std::optional<unsigned> & getXPosition() const;
        const std::optional<unsigned> & getYPosition() const;

        View & setSize(unsigned width, unsigned height);
        const std::optional<unsigned> & getWidth() const;
        const std::optional<unsigned> & getHeight() const;

        View & setText(const std::string & text);
        View & setText(std::wstring text);
        const std::optional<std::wstring> & getText() const;

        void setImpl(std::unique_ptr<ViewImpl> &&);
        ViewImpl* getImpl();

    private:
        std::unique_ptr<ViewImpl> m_impl;
        std::optional<unsigned> m_width;
        std::optional<unsigned> m_height;
        std::optional<unsigned> m_x_position;
        std::optional<unsigned> m_y_position;
        std::optional<std::wstring> m_text;
    };
}
