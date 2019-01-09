#include "Edit.h"
#include "EditImpl.h"
#include "ViewImpl.h"
#include "helper.h"
#include "Logger.h"

using namespace views_service::controls;

/*===========================================================================*/

EditImpl::EditImpl(ViewImpl & view_impl, Edit & edit) :
    m_view_impl(view_impl),
    m_edit(edit)
{
    TRACECALL();

    m_static_handle = CreateWindowW(
        L"STATIC",
        nullptr,
        WS_CHILD | WS_BORDER | WS_VISIBLE,
        m_edit.getXPosition().value(),
        m_edit.getYPosition().value(),
        m_edit.getWidth().value(),
        m_edit.getHeight().value(),
        m_view_impl.getHandle(),
        m_view_impl.getNextId(),
        m_view_impl.getModuleHandle(),
        nullptr);

    if (!m_static_handle) {
        std::runtime_error("CreateWindow(...) failed");
    }

    m_edit_handle = CreateWindowW(
        L"EDIT",
        m_edit.getText().value().data(),
        WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
        m_edit.getXPosition().value() + 2,
        m_edit.getYPosition().value() + (m_edit.getHeight().value() - m_view_impl.getFontHeight()) / 2,
        m_edit.getWidth().value() - 4,
        m_view_impl.getFontHeight(),
        m_view_impl.getHandle(),
        m_view_impl.getNextId(),
        m_view_impl.getModuleHandle(),
        nullptr);

    if (!m_edit_handle) {
        std::runtime_error("CreateWindow(...) failed");
    }
}

EditImpl::~EditImpl()
{
    TRACECALL();
}

/*===========================================================================*/

void EditImpl::setFont(const Font & font)
{
    SendMessage(m_edit_handle, WM_SETFONT, (WPARAM)static_cast<const HGDIOBJ>(font), TRUE);
}

/*===========================================================================*/