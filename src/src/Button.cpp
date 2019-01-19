#include "Button.h"	
#include "ButtonImpl.h"	

using namespace views_service::controls;

Button::Button()
{

}

Button::~Button()
{

}

Button::Button(Button && other) noexcept
{

}

Button & Button::setClickHandler(std::function<void()> click_handler)
{
    m_click_handler = click_handler;    
    return *this;
}

std::function<void()> & Button::getClickHandler()
{
    return m_click_handler;
}

Button & Button::setPosition(unsigned x_pos, unsigned y_pos)
{
    m_x_position = x_pos;
    m_y_position = y_pos;
    return *this;
}

const std::optional<unsigned> & Button::getXPosition() const
{
    return m_x_position;
}

const std::optional<unsigned> & Button::getYPosition() const
{
    return m_y_position;
}

Button & Button::setSize(unsigned width, unsigned height)
{
    m_width = width;
    m_height = height;
    return *this;
}

const std::optional<unsigned> & Button::getWidth() const
{
    return m_width;
}

const std::optional<unsigned> & Button::getHeight() const
{
    return m_height;
}

Button & Button::setText(const std::string & text)
{
    m_text = std::wstring(text.cbegin(), text.cend());
    return *this;
}

Button & Button::setText(std::wstring text)
{
    m_text = text;
    return *this;
}

const std::optional<std::wstring> & Button::getText() const
{
    return m_text;
}

void Button::setImpl(std::unique_ptr<ButtonImpl> && impl)
{
    m_impl.reset(impl.release());
}

ButtonImpl * Button::getImpl()
{
    return m_impl.get();
}
