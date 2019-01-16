#pragma once

#include <Windows.h>
#include <functional>
#include <exception>
#include <optional>
#include <map>

class Reactor
{
public:
    class IEventHandler;

    /**
     * Main events loop
     */
    int handleEvents();

    /**
     * If Reactor has unhandled exception throws it
     */
    void throwIfHasException();

    /**
     * Dispatches events to IEventHandler::onEvent(...)
     * Catches exception from IEventHandler::onEvent(...) and rethrows it in Reactor::throwIfHasException() after
     * 
     * [auto register]
     * WM_CREATE message:
     *   if OnEvent(...) not throws some exception and returns 0 then Reactor registers IEventHandler by HWND
     * WM_DESTROY message:
     *   Reactor unregister IEventHandler by HWND
     */
    LRESULT onEvent(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept;

    /**
     * Each window class should be use this WndProc
     * Dispatches messages from WndProc(...) to Reactor::onEvent(...)
     */
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept;

private:
    std::optional<std::exception_ptr> m_exception;
    std::map<HWND, std::reference_wrapper<IEventHandler>> m_hwnd_to_event_handler_map;
};

class Reactor::IEventHandler
{
public:
    IEventHandler(Reactor & reactor) : m_reactor(reactor) { }
    virtual ~IEventHandler() noexcept = default;

    /**
     * Needed for for auto register in Reactor by WM_CREATE message.
     */
    Reactor & getReactorInstance() { return m_reactor; }

    /**
     * Handles events from Reactor
     */
    virtual LRESULT onEvent(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) = 0;

private:
    Reactor & m_reactor;
};
