#pragma once

#include <initializer_list>
#include <list>
#include <string>
#include "Properties.h"

namespace views_service::controls
{
    class ComboBox : public Base<ComboBox>
    {
    public:
        ComboBox();
        ComboBox(ComboBox &&);
        ~ComboBox();

        /**
         * Adds text item. If text item exists does nothing.
         */
        ComboBox & addItem(std::wstring && item);

        /**
         * Remove text item. If text item doesn't exist does nothing.
         */
        ComboBox & eraseItem(std::wstring_view item);

        /**
         * Return selected text item. If selected item doesn't exist retruns empty.
         */
        std::wstring_view getSelectedItem();

        /**
         * Return text from edit.
         */
        std::wstring getInputText();

        /**
         * Item access
         */
        auto begin() const
        {
            return m_items.cbegin();
        }

        auto end() const
        {
            return m_items.cend();
        }

    private:
        std::list<std::wstring> m_items;
    };
}
