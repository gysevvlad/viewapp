#include <string>
#include "helper.h"

std::string getLastErrorMessage(std::string_view function_name) noexcept
{
    char* error_message_buffer;
    unsigned error_code = GetLastError();

    FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        error_code,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (char *)&error_message_buffer,
        0, nullptr);

    std::string error_message(function_name);
    error_message.append(" failed with error ");
    error_message.append(std::to_string(error_code));
    error_message.append(": ");
    error_message.append(error_message_buffer);
    LocalFree(error_message_buffer);
    return error_message;
}

std::wstring WindowClass::generate_class_name()
{
    static int id = 0;
    std::wstring class_name = L"window_class_";
    class_name.append(std::to_wstring(id++));
    return class_name;
}