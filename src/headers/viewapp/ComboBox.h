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

        enum class Type
        {
            // Drop down item list
            DROP_DONW,
            // Item list shows on static panel
            SIMPLE
        };

        /**
         * Sets type of combobox.
         */
        ComboBox & useDropDownType()
        {
            m_type = Type::DROP_DONW;
            return *this;
        }

        ComboBox & useSimpleType()
        {
            m_type = Type::SIMPLE;
            return *this;
        }

        Type getType()
        {
            return m_type;
        }

        /**
         * Adds text item. If text item exists or is empty does nothing.
         */
        ComboBox & addItem(std::wstring && item);

        /**
         * Remove text item. If text item doesn't exist or is empty does nothing.
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
        Type m_type;
    };
}
