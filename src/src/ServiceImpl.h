#pragma once

#include <Windows.h>
#include <string>

namespace views_service {

    class Service;

    namespace controls
    {
        class ViewImpl;
    }

    class ServiceImpl
    {
    public:
        ServiceImpl(Service & service);
        ServiceImpl(Service & service, const std::string & name);
        ~ServiceImpl();

        int run();

        BOOL OnCreateHandler(controls::ViewImpl & view_impl, HWND hwnd);
        void OnCloseHandler(HWND hwnd);
        void OnDestroyHandler(HWND hwnd);
        HBRUSH OnCtlColorHandler(HWND hwnd, HDC hdc, HWND hwndChild, int type);

        const std::wstring & getClassName();
        const HMODULE & getModuleHandle();
        Service & getService();

    private:
        std::wstring m_class_name;
        HMODULE m_module_handle;
        UINT m_style;
        HCURSOR m_cursor;
        HBRUSH m_background_brush;
        Service & m_service;
    };
}