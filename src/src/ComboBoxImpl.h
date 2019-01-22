#pragma once

#include "ComboBox.h"
#include "ViewImpl.h"

namespace views_service::controls
{
    template<>
    class Impl<ComboBox> : public ICommandHandler
    {
    public:
        Impl(ComboBox & m_combo_box, HWND parent, int id);

        virtual void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) override;

    private:
        ComboBox & m_combo_box;
        WindowHandle m_handle;
    };
}
