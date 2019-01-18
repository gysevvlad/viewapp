#pragma once

#include <Windows.h>
#include <windowsx.h>
#include <string>
#include <cassert>
#include <utility>
#include <functional>

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...)->overloaded<Ts...>;

std::string getLastErrorMessage(std::string_view function_name) noexcept;

class GDIObject
{
public:
    GDIObject(HGDIOBJ handle = nullptr) :
        m_handle(handle)
    {

    }

    GDIObject(GDIObject && other) : 
        m_handle(std::exchange(other.m_handle, nullptr))
    {

    }

    GDIObject & operator=(GDIObject && other)
    {
        std::swap(m_handle, other.m_handle);
        return *this;
    }

    operator HGDIOBJ() const
    {
        return m_handle;
    }

    ~GDIObject()
    {
        if (m_handle) {
            auto res = DeleteObject(m_handle);
            assert(res != 0);
        }
    }

private:
    HGDIOBJ m_handle;
};

using Brush = GDIObject;
using Font = GDIObject;

class WindowHandle
{
public:
    WindowHandle(HWND window_handle = nullptr)
        : m_window_handle(window_handle)
    {

    }

    WindowHandle(WindowHandle && other)
        : m_window_handle(std::exchange(other.m_window_handle, nullptr))
    {

    }

    WindowHandle & operator=(WindowHandle && other)
    {
        std::swap(m_window_handle, other.m_window_handle);
        return *this;
    }

    operator bool() const
    {
        return m_window_handle;
    }

    operator HWND() const
    {
        return m_window_handle;
    }

    ~WindowHandle()
    {
        if (m_window_handle) {
            auto res = DestroyWindow(m_window_handle);
            assert(res != 0);
        }
    }

    HWND get() const
    {
        return m_window_handle;
    }

private:
    HWND m_window_handle;
};

class DeviceContext
{
public:
    DeviceContext(const WindowHandle & window_handle = nullptr) :
        m_window_handle(window_handle.get()),
        m_device_context(window_handle ? GetDC(window_handle) : nullptr)
    {

    }

    DeviceContext(DeviceContext && other) :
        m_window_handle(std::exchange(other.m_window_handle, nullptr)),
        m_device_context(std::exchange(other.m_device_context, nullptr))
    {

    }

    ~DeviceContext()
    {
        if (m_window_handle && m_device_context) {
            auto res = ReleaseDC(m_window_handle, m_device_context);
            assert(res != 0);
        }
    }

    operator HDC() const
    {
        return m_device_context;
    }

private:
    HWND m_window_handle;
    HDC m_device_context;
};

class SelectObjectWrapper
{
public:
    SelectObjectWrapper(const DeviceContext & dc, const GDIObject & object) :
        m_device_context(dc),
        m_origin_object(SelectObject(dc, object))
    {

    }

    ~SelectObjectWrapper()
    {
        auto res = SelectObject(m_device_context, m_origin_object);
        assert(res != nullptr);
    }

private:
    const DeviceContext & m_device_context;
    HGDIOBJ m_origin_object;
};

class WindowClass
{
public:
    WindowClass(WNDPROC wndproc, std::wstring_view class_name = generate_class_name()) :
        m_class_name(class_name)
    {
        WNDCLASSW wc = { 0 };
        wc.lpszClassName = m_class_name.data();
        wc.lpfnWndProc = wndproc;
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = GetStockBrush(WHITE_BRUSH);
        wc.hInstance = GetModuleHandleW(nullptr);

        if (!RegisterClassW(&wc)) {
            std::runtime_error(getLastErrorMessage("RegisterClassW(...)"));
        }
    }

    const std::wstring & getClassName() { return m_class_name; }

    HMODULE getModuleHandle() { return GetModuleHandleW(nullptr); }

    ~WindowClass() noexcept
    {
        UnregisterClassW(m_class_name.data(), GetModuleHandleW(nullptr));
    }

private:
    static std::wstring generate_class_name();

private:
    WNDCLASSW m_wc;
    std::wstring m_class_name;
};
