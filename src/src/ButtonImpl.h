#pragma once

#include "Button.h"
#include "ViewImpl.h"

namespace views_service::controls
{
    template<>
    class Impl<Button> : public ViewImpl::ICommandHandler
    {
    public:
        Impl(Button & m_button, HWND parent, int id);

        virtual void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) override;

    private:
        Button & m_button;
        WindowHandle m_handle;
    };
}
