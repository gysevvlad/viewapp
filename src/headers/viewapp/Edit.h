#pragma once

#include "Properties.h"

namespace views_service::controls
{
    class Edit : public Base<Edit>
    {
    public:
        Edit();
        Edit(Edit && edit);
        ~Edit();

        std::wstring getInputText();
    };
}
