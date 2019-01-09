#pragma once

#include <list>
#include <memory>

#include "View.h"
#include "Logger.h"

namespace views_service
{
    class ServiceImpl;

    class Service
    {
    public:
        Service();
        Service(const std::string & name);
        ~Service();

        controls::View & createView();
        int run();

        std::list<controls::View>::iterator begin();
        std::list<controls::View>::iterator end();
        void erase(std::list<controls::View>::iterator pos);

        ServiceImpl & getImpl();

    private:
        std::unique_ptr<ServiceImpl> m_impl;
        std::list<controls::View> m_views;
    };
};