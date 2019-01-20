#pragma once


#include <string>
#include <map>
#include <functional>

#include <windows.h>

#include "View.h"
#include "Reactor.h"
#include "helper.h"

namespace views_service::controls
{
    class ViewImpl : public Reactor::IEventHandler
    {
    public:
        class ICommandHandler;

    public:
        ViewImpl(std::wstring_view class_name, View & m_view, Reactor & reactor);
        ~ViewImpl();

        void Close();

        virtual LRESULT onEvent(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;

        void onClose(HWND hwnd);
        void onDestroy(HWND hwnd);
        BOOL onCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);
        void onCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);

    private:
        int get_control_id();

    private:
        std::map<int, std::reference_wrapper<ViewImpl::ICommandHandler>> m_controls;
        WindowHandle m_handle;
        View & m_view;
        int m_id;
    };

    class ViewImpl::ICommandHandler
    {
    public:
        virtual void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) = 0;
        virtual ~ICommandHandler() = default;
    };
}
