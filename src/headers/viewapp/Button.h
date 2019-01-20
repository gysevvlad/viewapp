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

        /**
         * Implementation
         */
        class Impl;

        Impl * getImpl()
        {
            return m_impl.get();
        }

        void setImpl(std::unique_ptr<Impl> && impl);

    private:
        std::unique_ptr<Impl> m_impl;
        std::function<void()> m_click_handler;
    };
}
