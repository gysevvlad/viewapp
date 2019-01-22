#pragma once

#include "Properties.h"

namespace views_service::controls
{
    class ComboBox : public Base<ComboBox>
    {
    public:
        ComboBox();
        ComboBox(ComboBox &&);
        ~ComboBox();
    };
}
