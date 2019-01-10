#pragma once

#include <Windows.h>
#include <windowsx.h>

#include "helper.h"

namespace views_service::controls
{
    class Button;
    class ViewImpl;

    class ButtonImpl
    {
    public:
        ButtonImpl(ViewImpl & m_view_impl, Button & m_edit);
        ~ButtonImpl();

    private:
        WindowHandle m_handle;
        ViewImpl & m_view_impl;
        controls::Button & m_button;
    };
}