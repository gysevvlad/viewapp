#pragma once

#include <windows.h>

#include "helper.h"

namespace views_service 
{
    class ServiceImpl;
}

namespace views_service::controls
{
    class View;

    class ViewImpl
    {
    public:
        ViewImpl(ServiceImpl & m_service, View & m_view);
        ~ViewImpl();

        BOOL OnCreateHandler(HWND hwnd);
        void OnCloseHandler();
        HBRUSH OnCtlColorHandler(HDC hdc, HWND hwndChild, int type);

        ServiceImpl & getServiceImpl();
        const WindowHandle & getHandle();
        unsigned getFontHeight();
        HMENU getNextId();
        const HMODULE & getModuleHandle();

    private:
        void set_default_font();
        void set_default_background_brush();

    private:
        ServiceImpl & m_service;
        View & m_view;
        WindowHandle m_handle;
        GDIObject m_font;
        long long m_next_id;
        unsigned m_font_height;
        LOGBRUSH m_log_background_brush;
        HBRUSH m_background_brush;
    };
}