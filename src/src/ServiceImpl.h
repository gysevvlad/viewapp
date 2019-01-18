#pragma once

#include <string>

#include <Windows.h>

#include "Service.h"
#include "Reactor.h"
#include "helper.h"

namespace views_service {

    class ServiceImpl : public WindowClass
    {
    public:
        ServiceImpl(Service & service);
        ~ServiceImpl();

        int run();

        Service & getService();

    private:
        Reactor m_reactor;
        Service & m_service;
    };
}
