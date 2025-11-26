#pragma once

#include <filesystem>
#include <fstream>
#include <string>
#include <Windows.h>

long double getNumber(std::wstring string);
bool getBool(std::wstring string);
std::wstring ReadFileToWString(char* filePath);
std::wstring ToWString(const std::string& str);