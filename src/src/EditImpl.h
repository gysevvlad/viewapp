#pragma once

#include "Edit.h"
#include "ViewImpl.h"
#include "helper.h"

namespace views_service::controls
{
    class EditImpl : public ViewImpl::ICommandHandler
    {
    public:
        EditImpl(Edit & m_edit, HWND parent_handle, int control_id);

        virtual void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) override;

    private:
        WindowHandle m_edit_handle;
        controls::Edit & m_edit;
    };
}
