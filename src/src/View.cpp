#include "View.h"
#include "ViewImpl.h"

using namespace views_service::controls;

View::View()
{

}

View::~View()
{

}

View & View::setPosition(unsigned x_pos, unsigned y_pos)
{
    m_x_position = x_pos;
    m_y_position = y_pos;
    return *this;
}

const std::optional<unsigned> & View::getXPosition() const
{
    return m_x_position;
}

const std::optional<unsigned> & View::getYPosition() const
{
    return m_y_position;
}

View & View::setSize(unsigned width, unsigned height)
{
    m_width = width;
    m_height = height;
    return *this;
}

const std::optional<unsigned> & View::getWidth() const
{
    return m_width;
}

const std::optional<unsigned> & View::getHeight() const
{
    return m_height;
}

View & View::setText(const std::string & text)
{
    m_text = std::wstring(text.cbegin(), text.cend());
    return *this;
}

View & View::setText(std::wstring text)
{
    m_text = text;
    return *this;
}

const std::optional<std::wstring> & View::getText() const
{
    return m_text;
}

void View::setImpl(std::unique_ptr<ViewImpl> && impl)
{
    m_impl = std::move(impl);
}

ViewImpl * View::getImpl()
{
    return m_impl.get();
}