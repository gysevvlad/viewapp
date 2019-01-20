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

        /**
         * Implementation
         */
        class Impl;

        Impl * getImpl()
        {
            return m_impl.get();
        }

        void setImpl(std::unique_ptr<Impl> && impl);

    private:
        std::unique_ptr<Impl> m_impl;
    };
}
