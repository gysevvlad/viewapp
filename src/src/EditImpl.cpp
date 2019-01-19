#include "EditImpl.h"

using namespace views_service::controls;

Edit::Edit() = default;
Edit::~Edit() = default;
Edit::Edit(Edit && other) = default;

void Edit::setImpl(std::unique_ptr<EditImpl> && impl)
{
    m_impl = std::move(impl);
}

EditImpl::EditImpl(Edit & edit, HWND parent_handle, int control_id) :
    m_edit(edit)
{
    m_edit_handle = CreateWindowW(
        L"EDIT",
        m_edit.getText().value().data(),
        WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
        m_edit.getXPosition().value(),
        m_edit.getYPosition().value(),
        m_edit.getWidth().value(),
        m_edit.getHeight().value(),
        parent_handle,
        (HMENU)control_id,
        GetModuleHandleW(nullptr),
        nullptr);

    if (!m_edit_handle) {
        std::runtime_error(getLastErrorMessage("CreateWindow(...)"));
    }
}

void views_service::controls::EditImpl::OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{

}
