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

        ComboBox & addItems(std::initializer_list<const char *> items);

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
