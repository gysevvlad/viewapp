#include <iostream>
#include "Logger.h"

extern void(*g_log_handler)(const std::string & str) =
    [](const std::string & str) {
        std::clog << str;
    };