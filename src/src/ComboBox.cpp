#include "ComboBoxImpl.h"

using namespace views_service::controls;

ComboBox::ComboBox() = default;
ComboBox::ComboBox(ComboBox &&) = default;
ComboBox::~ComboBox() = default;

void Base<ComboBox>::setImpl(std::unique_ptr<Impl<ComboBox>> && impl)
{
    m_impl = std::move(impl);
}

Impl<ComboBox>::Impl(ComboBox & combo_box, HWND parent, int id) :
    m_combo_box(combo_box)
{
    m_handle = CreateWindowW(L"COMBOBOX",
        m_combo_box.getText().value().data(),
        WS_VISIBLE | WS_CHILD,
        m_combo_box.getXPosition().value(),
        m_combo_box.getYPosition().value(),
        m_combo_box.getWidth().value(),
        m_combo_box.getHeight().value(),
        parent, (HMENU)(void*)id, GetModuleHandleW(nullptr), nullptr);

    if (!m_handle) {
        throw std::runtime_error(getLastErrorMessage("CreateWindowW"));
    }
}

void Impl<ComboBox>::OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{

}
