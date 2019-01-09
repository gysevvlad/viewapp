#include <Windows.h>
#include <Windowsx.h>

#include <algorithm>

#include "helper.h"

#include "ViewImpl.h"
#include "View.h"
#include "ServiceImpl.h"
#include "Service.h"
#include "EditImpl.h"

using namespace views_service;
using namespace controls;

/*===========================================================================*/

ViewImpl::ViewImpl(ServiceImpl & service, View & view) :
    m_service(service),
    m_view(view)
{
    TRACECALL();

    auto handle = CreateWindowW(
        m_service.getClassName().data(),
        m_view.getText().value().data(),
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        m_view.getXPosition().value(),
        m_view.getYPosition().value(),
        m_view.getWidth().value() + 16,
        m_view.getHeight().value() + 38,
        nullptr, nullptr,
        m_service.getModuleHandle(),
        static_cast<LPVOID>(this));

    if (!handle) {
        throw std::runtime_error("CreateWindow(...) failed");
    }
}

ViewImpl::~ViewImpl()
{
    TRACECALL();

    for (auto & control : m_view) {
        std::visit(overloaded{
            [this](controls::Edit & edit) {
                edit.setImpl(nullptr);
            },
            [this](controls::Button & button) {
                throw std::runtime_error("Button control not released");
            }
        }, control);
    }
}

/*===========================================================================*/

BOOL ViewImpl::OnCreateHandler(HWND hwnd)
{
    TRACECALL();

    m_handle = hwnd;
    set_default_font();
    set_default_background_brush();
    
    for (auto & control : m_view) {
        std::visit(overloaded{
            [this](controls::Edit & edit) {
                auto edit_impl = std::make_unique<EditImpl>(*this, edit);
                edit_impl->setFont(m_font);
                edit.setImpl(std::move(edit_impl));
            },
            [this](controls::Button & button) {
                throw std::runtime_error("Button control not released");
            }
        }, control);
    }
    
    return TRUE;
}

void ViewImpl::OnCloseHandler()
{
    TRACECALL();

    m_view.setImpl(nullptr);
}

HBRUSH ViewImpl::OnCtlColorHandler(HDC hdc, HWND, int)
{
    TRACECALL();

    SetBkColor(hdc, m_log_background_brush.lbColor);
    return m_background_brush;
}

/*===========================================================================*/

void ViewImpl::set_default_font()
{
    LOGFONT lf;

    if (!GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf)) {
        std::runtime_error("GetObject(...) failed");
    }

    DeviceContext device_context(m_handle);

    lf.lfHeight = -MulDiv(10, GetDeviceCaps(device_context, LOGPIXELSY), 72);

    GDIObject font = CreateFontIndirect(&lf);

    RECT area;
    SelectObjectWrapper wrapper(device_context, font);

    if (!DrawTextW(device_context, L" ", 1, &area, DT_CALCRECT)) {
        std::runtime_error("DrawText(...) failed");
    }

    m_font = std::move(font);
    m_font_height = area.bottom - area.top;
}

void ViewImpl::set_default_background_brush()
{
    auto brush = GetSysColorBrush(COLOR_BTNFACE);

    if (brush == nullptr) {
        throw std::runtime_error("GetSysColorBrush(...) failed");
    }
    LOGBRUSH logBrush;

    if (GetObject(brush, sizeof(LOGBRUSH), &logBrush) == 0) {
        throw std::runtime_error("GetObject(...) failed");
    }

    m_log_background_brush = logBrush;
    m_background_brush = brush;
}

/*===========================================================================*/

ServiceImpl & ViewImpl::getServiceImpl()
{
    return m_service;
}

const WindowHandle & ViewImpl::getHandle()
{
    return m_handle;
}

unsigned ViewImpl::getFontHeight()
{
    return m_font_height;
}

HMENU ViewImpl::getNextId()
{
    return (HMENU)m_next_id++;
}

const HMODULE & ViewImpl::getModuleHandle()
{
    return m_service.getModuleHandle();
}

/*===========================================================================*/