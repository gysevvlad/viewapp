#include <Windows.h>
#include <Windowsx.h>

#include <algorithm>

#include "ViewImpl.h"

using namespace views_service;
using namespace controls;

ViewImpl::ViewImpl(std::wstring_view class_name, View & view, Reactor & reactor) :
    IEventHandler(reactor),
    m_view(view)
{
    // CreateWindowW -> Reactor::WndProc(...) -> ViewImpl::onCreate(...)
    m_handle = CreateWindowW(
        class_name.data(),
        m_view.getText().value().data(),
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        m_view.getXPosition().value(),
        m_view.getYPosition().value(),
        m_view.getWidth().value() + 16,
        m_view.getHeight().value() + 38,
        nullptr, nullptr,
        GetModuleHandleW(nullptr),
        this);

    getReactorInstance().throwIfHasException();

    if (!m_handle) {
        throw std::runtime_error("CreateWindow(...) failed");
    }
    m_view.setImpl(std::unique_ptr<ViewImpl>(this));
}

ViewImpl::~ViewImpl()
{

}

LRESULT ViewImpl::onEvent(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
        HANDLE_MSG(hWnd, WM_CLOSE, onClose);
        HANDLE_MSG(hWnd, WM_DESTROY, onDestroy);
        HANDLE_MSG(hWnd, WM_CREATE, onCreate);
        default: return DefWindowProcW(hWnd, message, wParam, lParam);
    }
}

void ViewImpl::onClose(HWND hwnd)
{
    // ~WindowClass() -> DestroyWindow(...) -> Reactor::WndProc -> ViewImpl::onDestroy()
    m_handle = nullptr;
}

void ViewImpl::onDestroy(HWND hwnd)
{
    m_view.setImpl(nullptr);
}

BOOL ViewImpl::onCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
    return TRUE;
}
