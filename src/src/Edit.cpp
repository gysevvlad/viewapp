#include "EditImpl.h"

using namespace views_service::controls;

Edit::Edit() = default;
Edit::Edit(Edit && other) = default;
Edit::~Edit() = default;

void Edit::setImpl(std::unique_ptr<Edit::Impl> && impl)
{
    m_impl = std::move(impl);
}

Edit::Impl::Impl(Edit & edit, HWND parent, int id) :
    m_edit(edit)
{
    m_handle = CreateWindowW(L"EDIT",
        m_edit.getText().value().data(),
        WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
        m_edit.getXPosition().value(),
        m_edit.getYPosition().value(),
        m_edit.getWidth().value(),
        m_edit.getHeight().value(),
        parent, (HMENU)id, GetModuleHandleW(nullptr), nullptr);

    if (!m_handle) {
        std::runtime_error(getLastErrorMessage("CreateWindow(...)"));
    }
}

void Edit::Impl::OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{

}
