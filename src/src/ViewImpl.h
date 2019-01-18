#pragma once

#include <windows.h>

#include <string>

#include "View.h"
#include "Reactor.h"
#include "helper.h"

namespace views_service::controls
{
    class ViewImpl : public Reactor::IEventHandler
    {
    public:
        ViewImpl(std::wstring_view class_name, View & m_view, Reactor & reactor);
        ~ViewImpl();

        virtual LRESULT onEvent(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
        void onClose(HWND hwnd);
        void onDestroy(HWND hwnd);
        BOOL onCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);

    private:
        WindowHandle m_handle;
        View & m_view;
    };
}