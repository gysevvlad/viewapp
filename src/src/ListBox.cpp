#include "ListBoxImpl.h"

using namespace views_service::controls;

ListBox::ListBox() = default;
ListBox::ListBox(ListBox && other) = default;
ListBox::~ListBox() = default;

void Base<ListBox>::setImpl(std::unique_ptr<Impl<ListBox>> && impl)
{
    m_impl = std::move(impl);
}

Impl<ListBox>::Impl(ListBox & list_box, HWND parent, int id) :
    m_list_box(list_box)
{
    m_handle = CreateWindowW(L"LISTBOX",
        m_list_box.getText().value().data(),
        WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
        m_list_box.getXPosition().value(),
        m_list_box.getYPosition().value(),
        m_list_box.getWidth().value(),
        m_list_box.getHeight().value(),
        parent, (HMENU)id, GetModuleHandleW(nullptr), nullptr);

    if (!m_handle) {
        std::runtime_error(getLastErrorMessage("CreateWindow(...)"));
    }
}

void Impl<ListBox>::OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{

}
