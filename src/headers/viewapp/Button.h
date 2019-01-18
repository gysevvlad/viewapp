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
        ~Button();
        Button(Button &&) noexcept;

        Button & setClickHandler(std::function<void()> click_handler);
        std::function<void()> & getClickHandler();

        Button & setPosition(unsigned x_pos, unsigned y_pos);
        const std::optional<unsigned> & getXPosition() const;
        const std::optional<unsigned> & getYPosition() const;

        Button & setSize(unsigned width, unsigned height);
        const std::optional<unsigned> & getWidth() const;
        const std::optional<unsigned> & getHeight() const;

        Button & setText(const std::string & text);
        Button & setText(std::wstring text);
        const std::optional<std::wstring> & getText() const;

        void setImpl(std::unique_ptr<ButtonImpl> && impl);
        ButtonImpl * getImpl();

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
