#include "Edit.h"
#include "EditImpl.h"
#include "Logger.h"

using namespace views_service::controls;

/*===========================================================================*/

Edit::Edit()
{
    TRACECALL();
}

Edit::~Edit()
{
    TRACECALL();
}

Edit::Edit(Edit &&) noexcept = default;

/*===========================================================================*/

Edit & Edit::setPosition(unsigned x_pos, unsigned y_pos)
{
    m_x_position = x_pos;
    m_y_position = y_pos;
    return *this;
}

const std::optional<unsigned> & Edit::getXPosition() const
{
    return m_x_position;
}

const std::optional<unsigned> & Edit::getYPosition() const
{
    return m_y_position;
}

Edit & Edit::setSize(unsigned width, unsigned height)
{
    m_width = width;
    m_height = height;
    return *this;
}

const std::optional<unsigned> & Edit::getWidth() const
{
    return m_width;
}

const std::optional<unsigned> & Edit::getHeight() const
{
    return m_height;
}

Edit & Edit::setText(const std::string & text)
{
    m_text = std::wstring(text.cbegin(), text.cend());
    return *this;
}

Edit & Edit::setText(std::wstring text)
{
    m_text = text;
    return *this;
}

const std::optional<std::wstring> & Edit::getText() const
{
    return m_text;
}

void Edit::setImpl(std::unique_ptr<EditImpl> && impl)
{
    m_impl.reset(impl.release());
}

EditImpl * Edit::getImpl()
{
    return m_impl.get();
}

/*===========================================================================*/