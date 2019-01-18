#include <Windows.h>
#include <windowsx.h>

#include "Service.h"
#include "ServiceImpl.h"

using namespace views_service;

Service::Service() : 
    m_impl(new ServiceImpl(*this))
{

}

Service::~Service()
{

}

int Service::run()
{
    return m_impl->run();
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
