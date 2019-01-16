#include <stdexcept>
#include <mutex>
#include <shared_mutex>
#include <map>
#include "Reactor.h"
#include "helper.h"

/**
 * Needed for dispatching messages from WndProc to Reactor instance
 */
std::shared_mutex g_hwnd_to_reactor_map_mutex;
std::map<HWND, std::reference_wrapper<Reactor>> g_hwnd_to_reactor_map;

LRESULT CALLBACK Reactor::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept
{
    if (message == WM_CREATE) {
        auto create_struct = static_cast<LPCREATESTRUCTW>(reinterpret_cast<void*>(lParam));
        auto& event_handler = *static_cast<Reactor::IEventHandler*>(create_struct->lpCreateParams);
        {
            std::unique_lock lock(g_hwnd_to_reactor_map_mutex);
            g_hwnd_to_reactor_map.emplace(hWnd, event_handler.getReactorInstance());
        }
        return event_handler.getReactorInstance().onEvent(hWnd, message, wParam, lParam);
    }
    else if (message == WM_DESTROY) {
        decltype(g_hwnd_to_reactor_map)::node_type node;
        {
            std::unique_lock lock(g_hwnd_to_reactor_map_mutex);
            node = g_hwnd_to_reactor_map.extract(hWnd);
        }
        return node.mapped().get().onEvent(hWnd, message, wParam, lParam);
    }
    else {
        std::shared_lock lock(g_hwnd_to_reactor_map_mutex);
        auto it = g_hwnd_to_reactor_map.find(hWnd);
        if (it != g_hwnd_to_reactor_map.end()) {
            auto& reactor = it->second.get();
            lock.unlock();
            return reactor.onEvent(hWnd, message, wParam, lParam);
        }
        lock.unlock();
        return DefWindowProcW(hWnd, message, wParam, lParam);;
    }
}

void Reactor::throwIfHasException()
{
    if (m_exception) {
        auto exception = m_exception.value();
        m_exception.reset();
        throw exception;
    }
}

int Reactor::handleEvents()
{
    MSG message;
    int ret;
    while (ret = GetMessageW(&message, nullptr, 0, 0)) {
        if (ret == -1) {
            throw std::runtime_error(getLastErrorMessage("GetMessageW(...)"));
        }
        else {
            TranslateMessage(&message);
            DispatchMessage(&message);
            throwIfHasException();
        }
    }
    return message.wParam;
}

LRESULT Reactor::onEvent(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept
{
    if (message == WM_CREATE) {
        auto create_struct = static_cast<LPCREATESTRUCTW>(reinterpret_cast<void*>(lParam));
        auto& event_handler = *static_cast<Reactor::IEventHandler*>(create_struct->lpCreateParams);
        try {
            auto return_value = event_handler.onEvent(hWnd, message, wParam, lParam);
            if (return_value == 0) {
                m_hwnd_to_event_handler_map.emplace(hWnd, event_handler);
            }
            return return_value;
        }
        catch (...) {
            m_exception = std::current_exception();
            return -1;
        }
    }
    else if (message == WM_DESTROY) {
        auto node = m_hwnd_to_event_handler_map.extract(hWnd);
        try {
            node.mapped().get().onEvent(hWnd, message, wParam, lParam);
        }
        catch (...) {
            m_exception = std::current_exception();
        }
        return 0;
    }
    else {
        try {
            return m_hwnd_to_event_handler_map.at(hWnd).get().onEvent(hWnd, message, wParam, lParam);
        }
        catch (...) {
            m_exception = std::current_exception();
            return DefWindowProcW(hWnd, message, wParam, lParam);
        }
    }
}
