#pragma once

#include <Windows.h>
#include <Windowsx.h>

#include "ServiceImpl.h"
#include "Service.h"
#include "ViewImpl.h"

using namespace views_service;

ServiceImpl::ServiceImpl(Service & service) :
    WindowClass(Reactor::WndProc),
    m_service(service)
{

}

ServiceImpl::~ServiceImpl()
{

}

int ServiceImpl::run()
{
    for (auto & view : m_service) {
        view.setImpl(std::make_unique<controls::Impl<controls::View>>(getClassName(), view, m_reactor));
    }
    return m_reactor.handleEvents();
}

Service & ServiceImpl::getService()
{
    return m_service;
}
