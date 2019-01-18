#include <Windows.h>
#include <windowsx.h>

#include "Service.h"
#include "ServiceImpl.h"

using namespace views_service;

Service::Service()
{

}

Service::~Service()
{

}

int Service::run()
{
    m_impl = std::make_unique<ServiceImpl>(*this);
    auto ret = m_impl->run();
    m_impl = nullptr;
    return ret;
}

controls::View & Service::createView()
{
    return m_views.emplace_back();
}

std::list<controls::View>::iterator Service::begin()
{ 
    return m_views.begin();
}

std::list<controls::View>::iterator Service::end()
{
    return m_views.end(); 
}

ServiceImpl & Service::getImpl()
{ 
    return *m_impl;
}
