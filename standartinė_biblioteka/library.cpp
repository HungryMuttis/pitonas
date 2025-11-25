#include <chrono>
#include <iostream>
#include <Windows.h>

typedef std::wstring(*PitonFunc)(std::vector<std::wstring>);
typedef void (*RegisterCallback)(void* ctx, const wchar_t* name, PitonFunc func);

#define REG(name, lambda) callback(ctx, name, lambda)

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
std::wstring formatNumber(long double number)
{
    std::wstring str = std::to_wstring(number);
    if (str.find(L'.') != std::wstring::npos)
    {
        str.erase(str.find_last_not_of(L'0') + 1, std::string::npos);
        if (str.back() == L'.') str.pop_back();
    }
    return str;
}
std::string ToString(const std::wstring& wstr)
{
    if (wstr.empty()) return std::string();
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
    std::string strTo(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);

    return strTo;
}

extern "C" __declspec(dllexport) void Load(void* ctx, RegisterCallback callback, const wchar_t* rawModuleName)
{
    std::wstring module = rawModuleName;

    if (module == L"konsolė")
    {
        REG(L"atspausdink", [](std::vector<std::wstring> args) -> std::wstring {
            for (const auto& string : args)
                std::wcout << string;
            std::wcout << std::endl;
            return L"";
            });

        REG(L"įvesk", [](std::vector<std::wstring> args) -> std::wstring {
            for (const auto& string : args)
                std::wcout << string;
            std::wstring line;
            std::getline(std::wcin, line);
            return line;
            });

        REG(L"palauk", [](std::vector<std::wstring> args) -> std::wstring {
            if (args.size() > 0) throw std::wstring(L"This function does not take in any arguments");
            system("pause");
            return L"";
            });
    }
    else if (module == L"aritmetika")
    {
        REG(L"sudėk", [](std::vector<std::wstring> args) -> std::wstring {
            if (args.size() < 2) throw std::wstring(L"This function takes in atleast 2 arguments");
            long double number = 0;
            for (const auto& string : args)
                number += getNumber(string);
            return formatNumber(number);
            });

        REG(L"atimk", [](std::vector<std::wstring> args) -> std::wstring {
            if (args.size() < 2) throw std::wstring(L"This function takes in atleast 2 arguments");
            long double number = getNumber(args[0]);
            for (size_t i = 1; i < args.size(); i++)
                number -= getNumber(args[i]);
            return formatNumber(number);
            });

        REG(L"padaugink", [](std::vector<std::wstring> args) -> std::wstring {
            if (args.size() < 2) throw std::wstring(L"This function takes in atleast 2 arguments");
            long double number = getNumber(args[0]);
            for (size_t i = 1; i < args.size(); i++)
                number *= getNumber(args[i]);
            return formatNumber(number);
            });

        REG(L"padalink", [](std::vector<std::wstring> args) -> std::wstring {
            if (args.size() < 2) throw std::wstring(L"This function takes in atleast 2 arguments");
            long double number = getNumber(args[0]);
            for (size_t i = 1; i < args.size(); i++)
                number /= getNumber(args[i]);
            return formatNumber(number);
            });
    }
    else if (module == L"logika")
    {
        REG(L"daugiau", [](std::vector<std::wstring> args) -> std::wstring {
            if (args.size() != 2) throw std::wstring(L"This function takes in 2 arguments");
            return getNumber(args[0]) > getNumber(args[1]) ? L"taip" : L"ne";
            });

        REG(L"mažiau", [](std::vector<std::wstring> args) -> std::wstring {
            if (args.size() != 2) throw std::wstring(L"This function takes in 2 arguments");
            return getNumber(args[0]) < getNumber(args[1]) ? L"taip" : L"ne";
            });

        REG(L"lygu", [](std::vector<std::wstring> args) -> std::wstring {
            if (args.size() != 2) throw std::wstring(L"This function takes in 2 arguments");
            return args[0] == args[1] ? L"taip" : L"ne";
            });

        REG(L"nelygu", [](std::vector<std::wstring> args) -> std::wstring {
            if (args.size() != 2) throw std::wstring(L"This function takes in 2 arguments");
            return args[0] != args[1] ? L"taip" : L"ne";
            });

        REG(L"ir", [](std::vector<std::wstring> args) -> std::wstring {
            if (args.size() < 2) throw std::wstring(L"This function takes in at least 2 arguments");
            for (const auto& string : args)
                if (!getBool(string)) return L"ne";
            return L"taip";
            });

        REG(L"arba", [](std::vector<std::wstring> args) -> std::wstring {
            if (args.size() < 2) throw std::wstring(L"This function takes in at least 2 arguments");
            for (const auto& string : args)
                if (getBool(string)) return L"taip";
            return L"ne";
            });

        REG(L"atvirkščiai", [](std::vector<std::wstring> args) -> std::wstring {
            if (args.size() != 1) throw std::wstring(L"This function takes in 1 argument");
            return getBool(args[0]) ? L"ne" : L"taip";
            });
    }
    else if (module == L"tipai")
    {
        REG(L"yra_skaičius", [](std::vector<std::wstring> args) -> std::wstring {
            if (args.size() != 1) throw std::wstring(L"This function takes in 1 argument");
            try
            {
                getNumber(args[0]);
                return L"taip";
            }
            catch (...)
            {
                return L"ne";
            }
            });

        REG(L"yra_loginis", [](std::vector<std::wstring> args) -> std::wstring {
            if (args.size() != 1) throw std::wstring(L"This function takes in 1 argument");
            try
            {
                getBool(args[0]);
                return L"taip";
            }
            catch (...)
            {
                return L"ne";
            }
            });
    }
    else if (module == L"sistema")
    {
        REG(L"laikas", [](std::vector<std::wstring> args) -> std::wstring {
            if (args.size() > 0) throw std::wstring(L"This function does not take in any arguments");
            return formatNumber(std::chrono::duration<long double>(std::chrono::system_clock::now().time_since_epoch()).count());
            });

        REG(L"komanada", [](std::vector<std::wstring> args) -> std::wstring {
            if (args.size() != 1) throw std::wstring(L"This function takes in 1 argument");
            std::string command = ToString(args[0]);
            system(command.c_str());
            return L"";
            });
    }
}