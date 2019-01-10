#include <Windows.h>
#include <windowsx.h>
#include <iostream>

#include "Logger.h"
#include "Service.h"
#include "ServiceImpl.h"
#include "ViewImpl.h"

using namespace views_service;

/*===========================================================================*/

Service::Service() : 
    m_impl(new ServiceImpl(*this))
{
    TRACECALL();
    
}

Service::Service(const std::string & name) : 
    m_impl(new ServiceImpl(*this, name))
{
    TRACECALL();
}

Service::~Service()
{
    TRACECALL();
}

/*===========================================================================*/

int Service::run()
{
    TRACECALL();
    return m_impl->run();
}

controls::View & Service::createView()
{
    TRACECALL();
    return m_views.emplace_back();
}

/*===========================================================================*/

std::list<controls::View>::iterator Service::begin()
{ 
    return m_views.begin();
}

std::list<controls::View>::iterator Service::end()
{
    return m_views.end(); 
}

void Service::erase(std::list<controls::View>::iterator pos)
{ 
    TRACECALL();
    m_views.erase(pos);
}

ServiceImpl & Service::getImpl()
{ 
    return *m_impl;
}

/*===========================================================================*/