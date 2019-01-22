#pragma once

#include "Properties.h"

namespace views_service::controls
{
    class ListBox : public Base<ListBox>
    {
    public:
        ListBox();
        ListBox(ListBox && edit);
        ~ListBox();
    };
}
