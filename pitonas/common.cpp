#include "common.h"

long double getNumber(std::wstring string)
{
    try
    {
        return stold(string);
    }
    catch (...)
    {
        throw std::wstring(L"Could not convert '" + string + L"' to a number");
    }
}
bool getBool(std::wstring string)
{
    if (string == L"taip") return true;
    else if (string == L"ne") return false;
    else throw std::wstring(L"This function takes in a boolean");
}

///////////////////////////////
// CODE MOSTLY WRITTEN BY AI //
///////////////////////////////

std::wstring ReadFileToWString(char* filePath)
{
    std::filesystem::path path(filePath);

    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file.is_open()) throw std::runtime_error("Could not open file");

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::string buffer(size, '\0');
    if (!file.read(&buffer[0], size)) throw std::runtime_error("Could not read file");

    if (buffer.empty()) return L"";

    if (buffer.size() >= 3 &&
        static_cast<unsigned char>(buffer[0]) == 0xEF &&
        static_cast<unsigned char>(buffer[1]) == 0xBB &&
        static_cast<unsigned char>(buffer[2]) == 0xBF)
    {
        buffer.erase(0, 3);
    }

    int wchars_num = MultiByteToWideChar(CP_UTF8, 0, buffer.c_str(), -1, NULL, 0);
    std::wstring wstr;
    wstr.resize(wchars_num - 1);
    MultiByteToWideChar(CP_UTF8, 0, buffer.c_str(), -1, &wstr[0], wchars_num);

    return wstr;
}
std::wstring ToWString(const std::string& str)
{
    if (str.empty()) return std::wstring();

    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
    return wstrTo;
}