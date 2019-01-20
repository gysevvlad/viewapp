#pragma once

#include <string>
#include <memory>
#include <optional>
#include <list>
#include <variant>

#include "Properties.h"
#include "Button.h"
#include "Edit.h"

namespace views_service::controls
{
    class View : public Base<View>
    {
    public:
        View();
        View(View &&);
        ~View();

        /**
         * Create button control
         */
        Button & createButton()
        {
            return std::get<Button>(m_controls.emplace_back(Button()));
        }

        /*
         * Create edit control
         */
        Edit & createEdit()
        {
            return std::get<Edit>(m_controls.emplace_back(Edit()));
        }

        /**
         * Close window
         */
        void Close();

        /**
         * Child controls
         */
        auto begin()
        {
            return m_controls.begin();
        }

        auto end()
        {
            return m_controls.end();
        }

    private:
        std::list<std::variant<Button, Edit>> m_controls;
    };
}
