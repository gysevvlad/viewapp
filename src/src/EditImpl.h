#pragma once

#include <Windows.h>
#include <windowsx.h>

#include "helper.h"

namespace views_service::controls
{
    class Edit;
    class ViewImpl;
    
    class EditImpl
    {
    public:
        EditImpl(ViewImpl & m_view_impl, Edit & m_edit);
        ~EditImpl();
    
        void setFont(const Font & font);
    
    private:
        WindowHandle m_edit_handle;
        WindowHandle m_static_handle;
        ViewImpl & m_view_impl;
        controls::Edit & m_edit;
    };
}