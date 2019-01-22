#include "ComboBoxImpl.h"

using namespace views_service::controls;

ComboBox::ComboBox() = default;
ComboBox::ComboBox(ComboBox &&) = default;
ComboBox::~ComboBox() = default;

void Base<ComboBox>::setImpl(std::unique_ptr<Impl<ComboBox>> && impl)
{
    m_impl = std::move(impl);
}

ComboBox & ComboBox::addItems(std::initializer_list<const char *> items)
{
    if (getImpl()) {
        for (auto item : items) {
            std::string_view temp(item);
            getImpl()->addItem(m_items.emplace_back(temp.begin(), temp.end()));
        }
    }
    else {
        for (auto item : items) {
            std::string_view temp(item);
            m_items.emplace_back(temp.begin(), temp.end());
        }
    }

    return *this;
}

void Impl<ComboBox>::addItem(std::wstring_view item)
{
    int err = SendMessageW(m_handle, CB_ADDSTRING, 0, (LPARAM)item.data());
    if (err == CB_ERR || err == CB_ERRSPACE) {
        throw std::runtime_error(getLastErrorMessage("SendMessageW"));
    }
}


Impl<ComboBox>::Impl(ComboBox & combo_box, HWND parent, int id) :
    m_combo_box(combo_box)
{
    m_handle = CreateWindowW(L"COMBOBOX",
        m_combo_box.getText().value().data(),
        WS_VSCROLL | WS_VISIBLE | WS_CHILD | CBS_SIMPLE | CBS_AUTOHSCROLL,
        m_combo_box.getXPosition().value(),
        m_combo_box.getYPosition().value(),
        m_combo_box.getWidth().value(),
        m_combo_box.getHeight().value() + 90,
        parent, (HMENU)(void*)id, GetModuleHandleW(nullptr), nullptr);

    if (!m_handle) {
        throw std::runtime_error(getLastErrorMessage("CreateWindowW"));
    }

    for (auto & item : m_combo_box) {
        addItem(item);
    }
}

void Impl<ComboBox>::OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{

}
