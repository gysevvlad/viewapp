#pragma once	

#include "ViewImpl.h"
#include "helper.h"

namespace views_service::controls
{
    class Button;

    class ButtonImpl : public ViewImpl::ICommandHandler
    {
    public:
        ButtonImpl(Button & m_button, HWND parent, int id);
        ~ButtonImpl();

        virtual void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) override;

    private:
        Button & m_button;
        WindowHandle m_handle;

    };
}