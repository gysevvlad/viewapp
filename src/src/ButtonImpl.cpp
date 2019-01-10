#include "Button.h"
#include "ButtonImpl.h"
#include "ViewImpl.h"
#include "helper.h"
#include "Logger.h"

using namespace views_service::controls;

/*===========================================================================*/

ButtonImpl::ButtonImpl(ViewImpl & view_impl, Button & button) :
    m_view_impl(view_impl),
    m_button(button)
{
    TRACECALL();
    throw std::runtime_error("ButtonImpl(...) failed");
}

ButtonImpl::~ButtonImpl()
{
    TRACECALL();
}

/*===========================================================================*/