#include "Logger.h"

void(*g_onLogHandler)(const std::string & str);

void views_service::setlogHandler(void(*log_handler)(const std::string & str))
{
    g_onLogHandler = log_handler;
    
}

void views_service::onLog(const std::string & str)
{
    g_onLogHandler(str);
}
