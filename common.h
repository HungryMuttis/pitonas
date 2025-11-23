#pragma once

#include <filesystem>
#include <fstream>
#include <string>
#include <Windows.h>

std::wstring formatNumber(long double number);
std::wstring ReadFileToWString(char* filePath);
std::wstring ToWString(const std::string& utf8Str);