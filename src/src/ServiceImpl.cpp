#pragma once

#include <Windows.h>
#include <Windowsx.h>

#include "ServiceImpl.h"
#include "Service.h"
#include "ViewImpl.h"

using namespace views_service;

ServiceImpl::ServiceImpl(Service & service) :
    WindowClass(L"views_service", Reactor::WndProc),
    m_service(service)
{

}

ServiceImpl::~ServiceImpl()
{

}

int ServiceImpl::run()
{
    for (auto & view : m_service) {
        std::make_unique<controls::ViewImpl>(getClassName(), view, m_reactor).release();
    }
    return m_reactor.handleEvents();
}

Service & ServiceImpl::getService()
{
    return m_service;
}
