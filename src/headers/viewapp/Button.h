#pragma once

#include <functional>
#include "Properties.h"

namespace views_service::controls
{
    class Button : public Base<Button>
    {
    public:
        Button();
        Button(Button &&);
        ~Button();

        /**
         * Click handler property
         */
        Button & setClickHandler(std::function<void()> click_handler)
        {
            m_click_handler = click_handler;
            return *this;
        }

        std::function<void()> & getClickHandler()
        {
            return m_click_handler;
        }

    private:
        std::function<void()> m_click_handler;
    };
}
