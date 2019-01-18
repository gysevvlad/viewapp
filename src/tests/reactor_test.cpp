#include <gtest/gtest.h>

#include "src/Reactor.h"
#include "src/helper.h"

template<class HoldFunctor>
class FunctorEventHandler : public Reactor::IEventHandler, HoldFunctor
{
public:
    template<class ParamFunctor>
    FunctorEventHandler(Reactor & reactor, ParamFunctor && functor) :
        HoldFunctor(std::forward<ParamFunctor>(functor)),
        IEventHandler(reactor)
    {

    }

    virtual LRESULT onEvent(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override
    {
        return HoldFunctor::operator()(hWnd, message, wParam, lParam);
    }
};

template<class Functor>
FunctorEventHandler(Reactor & reactor, Functor && functor)
    ->FunctorEventHandler<std::remove_reference_t<Functor>>;

HWND createTestWindow(std::wstring_view class_name, Reactor::IEventHandler * event_handler)
{
    return CreateWindowW(
        class_name.data(),
        L"test app",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT,
        nullptr, nullptr,
        GetModuleHandleW(nullptr),
        event_handler);
}

TEST(Reactor, BaseUsage)
{
    Reactor reactor;
    WindowHandle window_handle;
    auto message_counter = 0;
    auto handler = FunctorEventHandler{ reactor,
        [&window_handle, &message_counter] (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) -> LRESULT {
            if (message == WM_CREATE) {
                message_counter++;
                PostMessageW(hWnd, WM_CLOSE, 0, 0);
                return 0;
            }
            else if (message == WM_CLOSE) {
                message_counter++;
                window_handle = nullptr;
                return 0;
            }
            else if (message == WM_DESTROY) {
                message_counter++;
                return 0;
            }
            else {
                return DefWindowProcW(hWnd, message, wParam, lParam);
            }
        }
    };
    std::wstring class_name = L"BaseUsage";
    WindowClass window_class(class_name, Reactor::WndProc);
    window_handle = createTestWindow(class_name, &handler);
    ASSERT_NO_THROW(reactor.throwIfHasException());
    ASSERT_NE(window_handle, nullptr);
    ASSERT_EQ(reactor.handleEvents(), 0);
    ASSERT_EQ(message_counter, 3);
}

TEST(Reactor, WmCreateWithException)
{
    Reactor reactor;
    auto handler = FunctorEventHandler{ reactor,
        [](HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) -> LRESULT {
            if (message == WM_CREATE) {
                throw std::exception();
            }
            else {
                return DefWindowProcW(hWnd, message, wParam, lParam);
            }
        }
    };
    std::wstring class_name = L"BaseUsage";
    WindowClass window_class(class_name, Reactor::WndProc);
    auto window_handle = createTestWindow(class_name, &handler);
    ASSERT_ANY_THROW(reactor.throwIfHasException());
    ASSERT_EQ(window_handle, nullptr);
    ASSERT_EQ(reactor.handleEvents(), 0);
}

TEST(Reactor, WmCreateWithBadReturn)
{
    Reactor reactor;
    auto handler = FunctorEventHandler{ reactor,
        [](HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) -> LRESULT {
            if (message == WM_CREATE) {
                return -1;
            }
            else {
                return DefWindowProcW(hWnd, message, wParam, lParam);
            }
        }
    };
    std::wstring class_name = L"BaseUsage";
    WindowClass window_class(class_name, Reactor::WndProc);
    auto window_handle = createTestWindow(class_name, &handler);
    ASSERT_NO_THROW(reactor.throwIfHasException());
    ASSERT_EQ(window_handle, nullptr);
    ASSERT_EQ(reactor.handleEvents(), 0);
}

TEST(ReactorRegisterDeathTest, BaseUsage)
{
    Reactor reactor;
    auto handler = FunctorEventHandler{ reactor,
        [](HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) -> LRESULT {
            return 0;
        }
    };
    HWND hwnd = reinterpret_cast<HWND>(0x0F0F0F0F);
    CREATESTRUCTW create_struct;
    create_struct.lpCreateParams = &handler;
    Reactor::WndProc(hwnd, WM_CREATE, 0, reinterpret_cast<LPARAM>(&create_struct));
    Reactor::WndProc(hwnd, WM_APP, 0, 0);
    Reactor::WndProc(hwnd, WM_DESTROY, 0, 0);
    ASSERT_DEATH(reactor.onEvent(hwnd, WM_APP, 0, 0), "");
    ASSERT_NO_THROW(reactor.throwIfHasException());
    ASSERT_EQ(reactor.handleEvents(), 0);
}

TEST(ReactorRegisterDeathTest, DoubleRegister)
{
    Reactor reactor;
    auto handler = FunctorEventHandler{ reactor,
        [](HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) -> LRESULT {
            return 0;
        }
    };
    HWND hwnd = reinterpret_cast<HWND>(0x0F0F0F0F);
    CREATESTRUCTW create_struct;
    create_struct.lpCreateParams = &handler;
    Reactor::WndProc(hwnd, WM_CREATE, 0, reinterpret_cast<LPARAM>(&create_struct));      
    ASSERT_DEATH(Reactor::WndProc(hwnd, WM_CREATE, 0, reinterpret_cast<LPARAM>(&create_struct)), ".*");
    Reactor::WndProc(hwnd, WM_DESTROY, 0, 0);
    ASSERT_NO_THROW(reactor.throwIfHasException());
    ASSERT_EQ(reactor.handleEvents(), 0);
}

TEST(ReactorRegisterDeathTest, BadReturn)
{
    Reactor reactor;
    auto handler = FunctorEventHandler{ reactor,
        [](HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) -> LRESULT {
            return -1;
        }
    };
    HWND hwnd = reinterpret_cast<HWND>(0x0F0F0F0F);
    CREATESTRUCTW create_struct;
    create_struct.lpCreateParams = &handler;
    Reactor::WndProc(hwnd, WM_CREATE, 0, reinterpret_cast<LPARAM>(&create_struct));
    Reactor::WndProc(hwnd, WM_DESTROY, 0, 0);
    ASSERT_DEATH(reactor.onEvent(hwnd, WM_APP, 0, 0), "");
    ASSERT_NO_THROW(reactor.throwIfHasException());
    ASSERT_EQ(reactor.handleEvents(), 0);
}

TEST(ReactorRegisterDeathTest, Exception)
{
    Reactor reactor;
    auto handler = FunctorEventHandler{ reactor,
        [](HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) -> LRESULT {
            throw std::exception();
        }
    };
    HWND hwnd = reinterpret_cast<HWND>(0x0F0F0F0F);
    CREATESTRUCTW create_struct;
    create_struct.lpCreateParams = &handler;
    Reactor::WndProc(hwnd, WM_CREATE, 0, reinterpret_cast<LPARAM>(&create_struct));
    Reactor::WndProc(hwnd, WM_DESTROY, 0, 0);
    ASSERT_DEATH(reactor.onEvent(hwnd, WM_APP, 0, 0), "");
    ASSERT_ANY_THROW(reactor.throwIfHasException());
    ASSERT_EQ(reactor.handleEvents(), 0);
}

TEST(ReactorUnRegisterDeathTest, BaseUsage)
{
    Reactor reactor;
    auto handler = FunctorEventHandler{ reactor,
        [](HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) -> LRESULT {
            if (message == WM_APP) {
                return 12345;
            }
            else {
                return 0;
            }
        }
    };
    HWND hwnd = reinterpret_cast<HWND>(0x0F0F0F0F);
    CREATESTRUCTW create_struct;
    create_struct.lpCreateParams = &handler;
    Reactor::WndProc(hwnd, WM_CREATE, 0, reinterpret_cast<LPARAM>(&create_struct));
    ASSERT_EQ(Reactor::WndProc(hwnd, WM_APP, 0, 0), 12345);
    Reactor::WndProc(hwnd, WM_DESTROY, 0, 0);
    Reactor::WndProc(hwnd, WM_APP, 0, 0);
    ASSERT_DEATH(reactor.onEvent(hwnd, WM_APP, 0, 0), "");
    ASSERT_NO_THROW(reactor.throwIfHasException());
    ASSERT_EQ(reactor.handleEvents(), 0);
}

TEST(ReactorUnRegisterDeathTest, BadReturn)
{
    Reactor reactor;
    auto handler = FunctorEventHandler{ reactor,
        [](HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) -> LRESULT {
            if (message == WM_CREATE) {
                return 0;
            }
            else if (message == WM_DESTROY) {
                return -1;
            }
            else {
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
    };
    HWND hwnd = reinterpret_cast<HWND>(0x0F0F0F0F);
    CREATESTRUCTW create_struct;
    create_struct.lpCreateParams = &handler;
    Reactor::WndProc(hwnd, WM_CREATE, 0, reinterpret_cast<LPARAM>(&create_struct));
    Reactor::WndProc(hwnd, WM_DESTROY, 0, 0);
    Reactor::WndProc(hwnd, WM_APP, 0, 0);
    ASSERT_DEATH(reactor.onEvent(hwnd, WM_APP, 0, 0), "");
    ASSERT_NO_THROW(reactor.throwIfHasException());
    ASSERT_EQ(reactor.handleEvents(), 0);
}

TEST(ReactorUnRegisterDeathTest, Exception)
{
    Reactor reactor;
    auto handler = FunctorEventHandler{ reactor,
        [](HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) -> LRESULT {
            if (message == WM_CREATE) {
                return 0;
            }
            else if (message == WM_DESTROY) {
                throw std::exception();
            }
            else {
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
    };
    HWND hwnd = reinterpret_cast<HWND>(0x0F0F0F0F);
    CREATESTRUCTW create_struct;
    create_struct.lpCreateParams = &handler;
    Reactor::WndProc(hwnd, WM_CREATE, 0, reinterpret_cast<LPARAM>(&create_struct));
    Reactor::WndProc(hwnd, WM_DESTROY, 0, 0);
    Reactor::WndProc(hwnd, WM_APP, 0, 0);
    ASSERT_DEATH(reactor.onEvent(hwnd, WM_APP, 0, 0), "");
    ASSERT_ANY_THROW(reactor.throwIfHasException());
    ASSERT_EQ(reactor.handleEvents(), 0);
}

TEST(ReactorUnRegisterDeathTest, DoubleUnRegister)
{
    Reactor reactor;
    auto handler = FunctorEventHandler{ reactor,
        [](HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) -> LRESULT {
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    };
    HWND hwnd = reinterpret_cast<HWND>(0x0F0F0F0F);
    CREATESTRUCTW create_struct;
    create_struct.lpCreateParams = &handler;
    Reactor::WndProc(hwnd, WM_CREATE, 0, reinterpret_cast<LPARAM>(&create_struct));
    Reactor::WndProc(hwnd, WM_DESTROY, 0, 0);
    ASSERT_DEATH(Reactor::WndProc(hwnd, WM_DESTROY, 0, 0), "");
    ASSERT_NO_THROW(reactor.throwIfHasException());
    ASSERT_EQ(reactor.handleEvents(), 0);
}
