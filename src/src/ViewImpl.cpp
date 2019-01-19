#include <Windows.h>
#include <Windowsx.h>

#include <algorithm>

#include "ViewImpl.h"
#include "ButtonImpl.h"
#include "EditImpl.h"

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
        throw std::exception();
    }
}

ViewImpl::~ViewImpl()
{
    for (auto& variant : m_view) {
        std::visit([](auto& control) {
            control.setImpl(nullptr);
        },
        variant);
    }
}

void ViewImpl::Close()
{
    PostMessageW(m_handle, WM_CLOSE, 0, 0);
}

LRESULT ViewImpl::onEvent(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
        HANDLE_MSG(hWnd, WM_CLOSE, onClose);
        HANDLE_MSG(hWnd, WM_DESTROY, onDestroy);
        HANDLE_MSG(hWnd, WM_CREATE, onCreate);
        HANDLE_MSG(hWnd, WM_COMMAND, OnCommand);
        default: return DefWindowProcW(hWnd, message, wParam, lParam);
    }
}

void ViewImpl::OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    m_controls.at(id).get().OnCommand(hwnd, id, hwndCtl, codeNotify);
}

void ViewImpl::onClose(HWND hwnd)
{
    // ~WindowClass -> DestroyWindow -> Reactor::WndProc -> Reactor::onEvent -> ViewImpl::onDestroy
    m_handle = nullptr;
}

void ViewImpl::onDestroy(HWND hwnd)
{
    m_view.setImpl(nullptr);
}

BOOL ViewImpl::onCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
    for (auto& control : m_view) {
        std::visit(overloaded{
            [this, hwnd](Button & button) {
                auto id = get_control_id();
                ICommandHandler& control = *std::make_unique<ButtonImpl>(button, hwnd, id).release();
                m_controls.emplace(id, control);
            },
            [this, hwnd](Edit & edit) {
                auto id = get_control_id();
                auto edit_impl = std::make_unique<EditImpl>(edit, hwnd, id);
                ICommandHandler& control = *edit_impl;
                m_controls.emplace(id, control);
                edit.setImpl(std::move(edit_impl));
            }
            }, control);
    }
    return true;
}

int ViewImpl::get_control_id()
{
    static int id = 0;
    return id++;
}
