#pragma once

#include "Edit.h"
#include "ViewImpl.h"

namespace views_service::controls
{
    class EditImpl : public ViewImpl::ICommandHandler
    {
    public:
        EditImpl(Edit & m_edit, HWND parent_handle, int control_id);

        virtual void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) override;

    private:
        Edit & m_edit;
        WindowHandle m_handle;
    };
}
