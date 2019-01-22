#pragma once

#include "ListBox.h"
#include "ViewImpl.h"

namespace views_service::controls
{
    template<>
    class Impl<ListBox> : public ICommandHandler
    {
    public:
        Impl(ListBox & list_box, HWND parent_handle, int control_id);

        virtual void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) override;

    private:
        ListBox & m_list_box;
        WindowHandle m_handle;
    };
}
