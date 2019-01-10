#pragma once

#include <string>
#include <sstream>
#include <thread>

#define LOGGER(expression)          \
{                                   \
    std::stringstream ss;           \
    ss << "[thread:" << std::this_thread::get_id() << "]" << expression << std::endl;  \
    views_service::onLog(ss.str()); \
}

#define TRACECALL() LOGGER("[this:" << this << "] " << __FUNCTION__)

#define STRACECALL() LOGGER(" ::" << __FUNCTION__)

#define STRACELOGGER(expression) LOGGER(" ::" << __FUNCTION__ << " " << expression)

namespace views_service
{
    void setlogHandler(void(*)(const std::string & str));
    void onLog(const std::string & str);
}

