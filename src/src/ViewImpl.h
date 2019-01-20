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
    class ICommandHandler
    {
    public:
        virtual void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) = 0;
        virtual ~ICommandHandler() = default;
    };

    template<>
    class Impl<View> : public Reactor::IEventHandler
    {
    public:
        Impl(std::wstring_view class_name, View & m_view, Reactor & reactor);
        ~Impl();

        void Close();

        virtual LRESULT onEvent(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;

        void onClose(HWND hwnd);
        void onDestroy(HWND hwnd);
        BOOL onCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);
        void onCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);

    private:
        int get_control_id();

    private:
        std::map<int, std::reference_wrapper<ICommandHandler>> m_controls;
        WindowHandle m_handle;
        View & m_view;
        int m_id;
    };
}
