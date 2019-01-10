#pragma once

#include <string>
#include <sstream>
#include <thread>

extern void(*g_log_handler)(const std::string & str);

#define LOGGER(expression)          \
{                                   \
    std::stringstream ss;           \
    ss << "[thread:" << std::this_thread::get_id() << "]" << expression << std::endl;  \
    g_log_handler(ss.str()); \
}

#define TRACECALL() LOGGER("[this:" << this << "] " << __FUNCTION__)

#define STRACECALL() LOGGER(" ::" << __FUNCTION__)

#define STRACELOGGER(expression) LOGGER(" ::" << __FUNCTION__ << " " << expression)