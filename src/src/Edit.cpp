#include "EditImpl.h"

using namespace views_service::controls;

Edit::Edit() = default;
Edit::Edit(Edit && other) = default;
Edit::~Edit() = default;

void Base<Edit>::setImpl(std::unique_ptr<Impl<Edit>> && impl)
{
    m_impl = std::move(impl);
}

Impl<Edit>::Impl(Edit & edit, HWND parent, int id) :
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

void Impl<Edit>::OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{

}

std::wstring Edit::getInputText()
{
    if (getImpl()) {
        return getImpl()->getInputText();
    }
    return {};
}

std::wstring Impl<Edit>::getInputText()
{
    auto length = SendMessageW(m_handle, WM_GETTEXTLENGTH, 0, 0);
    std::unique_ptr<wchar_t[]> buffer(new wchar_t[length + 1]);
    SendMessageW(m_handle, WM_GETTEXT, sizeof(buffer), LPARAM(buffer.get()));
    return { buffer.get() };
}
