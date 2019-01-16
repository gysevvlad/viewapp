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
        100,
        100,
        200,
        200,
        nullptr, nullptr,
        GetModuleHandleW(nullptr),
        event_handler);
}

TEST(Reactor, BaseUsage)
{
    auto expected_return_value = 192837465;
    auto message_counter = 0;
    Reactor reactor;
    auto handler = FunctorEventHandler{ reactor,
        [expected_return_value, &message_counter] (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) -> LRESULT {
            if (message == WM_CREATE) {
                std::cout << "WM_CREATE" << std::endl;
                message_counter++;
                PostMessageW(hWnd, WM_CLOSE, 0, 0);
                return 0;
            }
            else if (message == WM_CLOSE) {
                std::cout << "WM_CLOSE" << std::endl;
                message_counter++;
                DestroyWindow(hWnd);
                return 0;
            }
            else if (message == WM_DESTROY) {
                std::cout << "WM_DESTROY" << std::endl;
                message_counter++;
                PostQuitMessage(expected_return_value);
                return 0;
            }
            else {
                return DefWindowProcW(hWnd, message, wParam, lParam);
            }
        }
    };
    std::wstring class_name = L"BaseUsage";
    WindowClass window_class(class_name, Reactor::WndProc);
    auto window_handle = createTestWindow(class_name, &handler);
    ASSERT_NE(window_handle, nullptr);
    ASSERT_EQ(reactor.handleEvents(), expected_return_value);
    ASSERT_EQ(message_counter, 3);
}
