#pragma once

#include <Windows.h>
#include <Windowsx.h>

#include <algorithm>
#include <unordered_map>

#include "ServiceImpl.h"
#include "Service.h"
#include "ViewImpl.h"

using namespace views_service;

/*===========================================================================*/

std::unordered_map<HWND, std::reference_wrapper<ServiceImpl>> g_window_handle_to_service_impl;

/*===========================================================================*/

BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
    STRACECALL();
    auto view_impl = static_cast<views_service::controls::ViewImpl*>(lpCreateStruct->lpCreateParams);
    return view_impl->getServiceImpl().OnCreateHandler(*view_impl, hwnd);
}

void OnDestroy(HWND hwnd)
{
    STRACECALL();
    auto & service_impl = g_window_handle_to_service_impl.at(hwnd).get();
    service_impl.OnDestroyHandler(hwnd);
}

HBRUSH OnCtlColor(HWND hwnd, HDC hdc, HWND hwndChild, int type)
{
    auto & service_impl = g_window_handle_to_service_impl.at(hwnd).get();
    return service_impl.OnCtlColorHandler(hwnd, hdc, hwndChild, type);
}

void OnClose(HWND hwnd)
{
    STRACECALL();
    auto & service_impl = g_window_handle_to_service_impl.at(hwnd).get();
    service_impl.OnCloseHandler(hwnd);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    try {
        switch (message)
        {
            HANDLE_MSG(hWnd, WM_CLOSE, OnClose);
            HANDLE_MSG(hWnd, WM_CTLCOLOREDIT, OnCtlColor);
            HANDLE_MSG(hWnd, WM_CTLCOLORSTATIC, OnCtlColor);
            HANDLE_MSG(hWnd, WM_CREATE, OnCreate);
            HANDLE_MSG(hWnd, WM_DESTROY, OnDestroy);
        default:
            return DefWindowProcW(hWnd, message, wParam, lParam);
        }
    }
    catch (std::exception e) {
        MessageBoxA(hWnd, e.what(), nullptr, MB_OK);
        return 0;
    }
}

/*===========================================================================*/

ServiceImpl::ServiceImpl(Service & service) :
    ServiceImpl(service, "views_service")
{

}

ServiceImpl::ServiceImpl(Service & service, const std::string & name) :
    m_class_name(name.begin(), name.end()),
    m_module_handle(GetModuleHandle(nullptr)),
    m_style(CS_HREDRAW | CS_VREDRAW),
    m_cursor(LoadCursor(NULL, IDC_ARROW)),
    m_background_brush(GetStockBrush(WHITE_BRUSH)),
    m_service(service)

{
    TRACECALL();
    WNDCLASSW wc = { 0 };
    wc.lpfnWndProc =   WndProc;
    wc.hInstance =     m_module_handle;
    wc.lpszClassName = m_class_name.data();
    wc.style =         m_style;
    wc.hCursor =       m_cursor;
    wc.hbrBackground = m_background_brush;

    if (!RegisterClassW(&wc)) {
        std::runtime_error("RegisterClassW(...) failed");
    }
}

ServiceImpl::~ServiceImpl()
{
    TRACECALL();
    auto res = UnregisterClassW(m_class_name.data(), m_module_handle);
    assert(res != 0);
}

/*===========================================================================*/

int ServiceImpl::run()
{
    TRACECALL();

    for (auto & view : m_service) {
        view.setImpl(std::make_unique<controls::ViewImpl>(*this, view));
    }

    MSG msg;

    while (GetMessageW(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    
    return (int)msg.wParam;
}

/*===========================================================================*/

BOOL ServiceImpl::OnCreateHandler(controls::ViewImpl & view_impl, HWND hwnd)
{
    TRACECALL();

    g_window_handle_to_service_impl.emplace(hwnd, *this);
    return view_impl.OnCreateHandler(hwnd);
}

void ServiceImpl::OnCloseHandler(HWND hwnd)
{
    TRACECALL();

    for (auto & view : m_service) {
        if (view.getImpl()) {
            if (view.getImpl()->getHandle() == hwnd) {
                view.getImpl()->OnCloseHandler();
                return;
            }
        }
    }
}

void ServiceImpl::OnDestroyHandler(HWND hwnd)
{
    TRACECALL();

    g_window_handle_to_service_impl.erase(hwnd);

    bool is_last_view = std::all_of(m_service.begin(), m_service.end(),
        [](auto & view) {
            return view.getImpl() == nullptr;
        });

    if (is_last_view) {
        PostQuitMessage(0);
    }
}

HBRUSH ServiceImpl::OnCtlColorHandler(HWND hwnd, HDC hdc, HWND hwndChild, int type)
{
    for (auto & view : m_service) {
        if (view.getImpl()) {
            if (view.getImpl()->getHandle() == hwnd) {
                return view.getImpl()->OnCtlColorHandler(hdc, hwndChild, type);
            }
        }
    }
    return nullptr;
}

/*===========================================================================*/

const std::wstring & ServiceImpl::getClassName()
{
    return m_class_name;
}

const HMODULE & ServiceImpl::getModuleHandle()
{
    return m_module_handle;
}

Service & ServiceImpl::getService()
{
    return m_service;
}

/*===========================================================================*/