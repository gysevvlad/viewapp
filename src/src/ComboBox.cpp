#include "ComboBoxImpl.h"

using namespace views_service::controls;

ComboBox::ComboBox() = default;
ComboBox::ComboBox(ComboBox &&) = default;
ComboBox::~ComboBox() = default;

void Base<ComboBox>::setImpl(std::unique_ptr<Impl<ComboBox>> && impl)
{
    m_impl = std::move(impl);
}


ComboBox & ComboBox::addItem(std::wstring && item)
{
    if (!item.empty()) {
        if (auto it = std::find(m_items.begin(), m_items.end(), item); it == m_items.end()) {
            auto& value = m_items.emplace_back(std::move(item));
            if (getImpl()) {
                getImpl()->addItem(value);
            }
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

ComboBox & ComboBox::eraseItem(std::wstring_view item)
{
    if (getImpl()) {
        if (auto it = std::find(m_items.begin(), m_items.end(), item); it != m_items.end()) {
            getImpl()->eraseItem(std::distance(m_items.begin(), it));
            m_items.erase(it);
        }
    }
    return *this;
}

void Impl<ComboBox>::eraseItem(int index)
{
    SendMessageW(m_handle, CB_DELETESTRING, index, 0);
}

std::wstring_view ComboBox::getSelectedItem()
{
    if (getImpl()) {
        auto index = getImpl()->getSelectedItem();
        if (index != -1) {
            return *std::next(m_items.begin(), index);
        }
    }

    return {};
}

int Impl<ComboBox>::getSelectedItem()
{
    int index = SendMessageW(m_handle, CB_GETCURSEL, 0, 0);
    return index != CB_ERR ? index : -1;
}

std::wstring ComboBox::getInputText()
{
    if (getImpl()) {
        return getImpl()->getInputText();
    }
}

std::wstring Impl<ComboBox>::getInputText()
{
    COMBOBOXINFO info = { sizeof(COMBOBOXINFO) };
    GetComboBoxInfo(m_handle, &info);
    wchar_t buffer[128] = { 0 };
    GetWindowTextW(info.hwndItem, buffer, 127);
    std::wstring value(buffer);
    return value;
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
        m_combo_box.getHeight().value(),
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
