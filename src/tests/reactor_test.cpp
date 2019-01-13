#include <gtest/gtest.h>

#include "src/Reactor.h"
#include "src/helper.h"

HWND createTestWindow(std::wstring_view class_name, void * custom_data = nullptr)
{
    return CreateWindowW(
        class_name.data(),
        L"test app",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        nullptr, nullptr,
        GetModuleHandleW(nullptr),
        custom_data);
}

TEST(Reactor, BaseUsage)
{
    std::wstring class_name = L"BaseUsage";
    int expected_return_code = 1234567;
    Reactor reactor;/*
    reactor.registerEventHandler(class_name,
        [expected_return_code] (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) -> LRESULT {
            if (message == WM_CREATE) {
                PostMessageW(hWnd, WM_CLOSE, 0, 0);
                return 0;
            } 
            else if (message == WM_DESTROY) {
                PostQuitMessage(expected_return_code);
                return 0;
            }
            else {
                return DefWindowProcW(hWnd, message, wParam, lParam);
            }
        });*/

    createTestWindow(class_name);
    ASSERT_EQ(reactor.handleEvents(), expected_return_code);
}

TEST(Reactor, OwnedWindowClass)
{
    auto class_name = L"OwnedWindowClass";
    {
        Reactor reactor;
        reactor.registerWindowClass(class_name);
        reactor.registerWindowClass(class_name);
    }
    WNDCLASSW wc = { 0 };
    ASSERT_FALSE(GetClassInfoW(GetModuleHandleW(nullptr), class_name, &wc));
    ASSERT_EQ(GetLastError(), ERROR_CLASS_DOES_NOT_EXIST);
}

TEST(Reactor, WindowClassWrongWndProc)
{
    auto class_name = L"WindowClassWrongWndProc";
    WNDCLASSW wc = { 0 };
    wc.lpfnWndProc = DefWindowProcW;
    wc.hInstance = GetModuleHandleW(nullptr);
    wc.lpszClassName = class_name;
    ASSERT_TRUE(RegisterClassW(&wc));
    Reactor reactor;
    ASSERT_ANY_THROW(reactor.registerWindowClass(class_name));
}