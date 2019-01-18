#include "Button.h"
#include "ButtonImpl.h"
#include "helper.h"

using namespace views_service::controls;

ButtonImpl::ButtonImpl(Button & button, HWND parent, int id) :
    m_button(button)
{
    m_handle = CreateWindowW(L"BUTTON",
        button.getText().value().data(),
        WS_VISIBLE | WS_CHILD,
        button.getXPosition().value(),
        button.getYPosition().value(),
        button.getWidth().value(),
        button.getHeight().value(),
        parent, (HMENU)(void*)id, GetModuleHandleW(nullptr), nullptr);

    if (!m_handle) {
        throw std::runtime_error(getLastErrorMessage("CreateWindowW"));
    }
}

ButtonImpl::~ButtonImpl()
{

}

void views_service::controls::ButtonImpl::OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    if (codeNotify == BN_CLICKED) {
        m_button.getClickHandler()();
    }
}
