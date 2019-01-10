#include "View.h"
#include "ViewImpl.h"
#include "Logger.h"

using namespace views_service::controls;

View::View()
{
    TRACECALL();
}

View::~View()
{
    TRACECALL();
}

Edit & View::createEdit()
{
    TRACECALL();
    return std::get<Edit>(m_controls.emplace_back(Edit()));
}

Button & View::createButton()
{
    TRACECALL();
    return std::get<Button>(m_controls.emplace_back(Button()));
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

std::list<std::variant<Edit, Button>>::iterator View::begin()
{ 
    return m_controls.begin();
}

std::list<std::variant<Edit, Button>>::iterator View::end()
{
    return m_controls.end();
}

void View::setImpl(std::unique_ptr<ViewImpl> && impl)
{
    m_impl = std::move(impl);
}

ViewImpl * View::getImpl()
{
    return m_impl.get();
}