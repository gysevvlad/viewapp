#pragma once

#include "Edit.h"
#include "ViewImpl.h"

namespace views_service::controls
{
    template<>
    class Impl<Edit> : public ICommandHandler
    {
    public:
        Impl(Edit & m_edit, HWND parent_handle, int control_id);

        virtual void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) override;

    private:
        Edit & m_edit;
        WindowHandle m_handle;
    };
}
