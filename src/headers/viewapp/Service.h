#pragma once

#include <list>
#include <memory>

#include "View.h"

namespace views_service
{
    class ServiceImpl;

    class Service
    {
    public:
        Service();
        ~Service();

        controls::View & createView();
        int run();

        std::list<controls::View>::iterator begin();
        std::list<controls::View>::iterator end();

        ServiceImpl & getImpl();

    private:
        std::unique_ptr<ServiceImpl> m_impl;
        std::list<controls::View> m_views;
    };
};
