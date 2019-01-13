#include <stdexcept>
#include "Reactor.h"
#include "helper.h"

std::mutex Reactor::g_class_name_to_reactor_map_mutex;
std::map<std::wstring, std::reference_wrapper<Reactor>> Reactor::g_class_name_to_reactor_map;

LRESULT CALLBACK Reactor::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return DefWindowProcW(hWnd, message, wParam, lParam);
}

void Reactor::registerWindowClass(std::wstring_view class_name)
{
    WNDCLASSW wc = { 0 };
    if (!GetClassInfoW(GetModuleHandleW(nullptr), class_name.data(), &wc)) {
        if (GetLastError() == ERROR_CLASS_DOES_NOT_EXIST) {
            SetLastError(ERROR_SUCCESS);
            wc.lpfnWndProc = Reactor::WndProc;
            wc.hInstance = GetModuleHandleW(nullptr);
            wc.lpszClassName = class_name.data();

            if (!RegisterClassW(&wc)) {
                std::runtime_error(getLastErrorMessage("RegisterClassW(...)"));
            }
            m_owned_class_windows.emplace(class_name);
        }
        else {
            throw std::runtime_error(getLastErrorMessage("GetClassInfoW(...)"));
        }
    }
    else if (wc.lpfnWndProc != Reactor::WndProc) {
        throw std::invalid_argument("Window class handler (WndProc) should be Reactor::WndProc");
    }
    {
        std::lock_guard lock(g_class_name_to_reactor_map_mutex);
        g_class_name_to_reactor_map.emplace(class_name, *this);
    }
}

void Reactor::unregisterWindowClass(std::wstring_view class_name)
{
    {
        std::lock_guard lock(g_class_name_to_reactor_map_mutex);
        auto it = g_class_name_to_reactor_map.find(class_name.data());

        if (it == g_class_name_to_reactor_map.end()) {
            throw std::invalid_argument("Window class doesn't exist");
        }
        if (&it->second.get() != this) {
            throw std::invalid_argument("Window class is associated with other Reactor instance");
        }
        g_class_name_to_reactor_map.erase(it);
    }
    if (auto it = m_owned_class_windows.find(class_name.data()); it != m_owned_class_windows.end()) {
        UnregisterClassW(it->data(), GetModuleHandleW(nullptr));
        m_owned_class_windows.erase(it);
    }
}

int Reactor::handleEvents()
{
    return 0;
}

Reactor::~Reactor() noexcept
{
    for (auto& class_name : m_owned_class_windows) {
        UnregisterClassW(class_name.data(), GetModuleHandleW(nullptr));
    }
}