#pragma once

#include <Windows.h>

#include <functional>
#include <set>
#include <string>
#include <string_view>
#include <map>
#include <mutex>

class Reactor
{
public:
    class IEventHandler
    {
        LRESULT onEvent(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) = delete;
    };

    using EventHandler = LRESULT(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
    /**
     * Associates window class with Reactor instance.
     *
     * If window class with name eq class_name is not
     * exists reactor registres new windows class
     */
    void registerWindowClass(std::wstring_view class_name);
    void unregisterWindowClass(std::wstring_view class_name);

    int handleEvents();

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    
    static std::mutex g_class_name_to_reactor_map_mutex;
    static std::map<std::wstring, std::reference_wrapper<Reactor>> g_class_name_to_reactor_map;

    ~Reactor() noexcept;
private:
    std::set<std::wstring> m_owned_class_windows;
};

/*
LRESULT onEvent(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT onWindowCreateEvent(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept;
LRESULT onWindowDestroyEvent(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept;

static LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
static LRESULT WndProcWmDestroy(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
static LRESULT WndProcWMCraete(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
    static std::map<HWND, std::reference_wrapper<Reactor>> g_hwnd_to_reactor_map;
    static std::map<std::wstring, std::reference_wrapper<Reactor>> g_class_name_to_reactor_map;

    std::map<std::wstring, std::function<EventHandler>> m_class_name_to_event_handler_map;
    std::map<HWND, std::reference_wrapper<std::function<EventHandler>>> m_hwnd_to_event_handler_map;

    std::optional<std::exception_ptr> m_exception;
    std::function<EventHandler> m_event_handler;
*/