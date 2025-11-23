#include "common.h"
#include "console.h"
#include "runtime.h"

static void registerBuiltins(Storage& builtins)
{
    // printing
    builtins.add(L"atspausdink", [](std::vector<std::wstring> args) {
        for (std::wstring string : args)
            std::wcout << string;
        std::wcout << std::endl;
        return L"";
    });

    // waiting
    builtins.add(L"palauk", [](std::vector<std::wstring> args) {
        if (args.size() > 0) throw std::wstring(L"This function does not take in any arguments");
        system("pause");
        return L"";
    });

    // input
    builtins.add(L"įvesk", [](std::vector<std::wstring> args) {
        for (std::wstring string : args)
            std::wcout << string;
        std::wstring line;
        std::getline(std::wcin, line);
        return line;
    });

    // arithmetic
    builtins.add(L"sudėk", [](std::vector<std::wstring> args) {
        if (args.size() < 2) throw std::wstring(L"This function takes in atleast 2 arguments");
        long double number = 0;
        try
        {
            for (std::wstring string : args)
                number += stold(string);
        }
        catch (...)
        {
            throw std::wstring(L"Could not convert variables to numbers");
        }
        return formatNumber(number);
    });
    builtins.add(L"atimk", [](std::vector<std::wstring> args) {
        if (args.size() != 2) throw std::wstring(L"This function takes in 2 arguments");
        try
        {
            return formatNumber(stold(args[0]) - stold(args[1]));
        }
        catch (...)
        {
            throw std::wstring(L"Could not convert variables to numbers");
        }
    });
    builtins.add(L"padaugink", [](std::vector<std::wstring> args) {
        if (args.size() < 2) throw std::wstring(L"This function takes in atleats 2 arguments");
        long double number = 0;
        try
        {
            number = stold(args[0]);
            for (int i = 1; i < args.size(); i++)
                number *= stold(args[i]);
        }
        catch (...)
        {
            throw std::wstring(L"Could not convert variables to numbers");
        }
        return formatNumber(number);
    });
    builtins.add(L"padalink", [](std::vector<std::wstring> args) {
        if (args.size() != 2) throw std::wstring(L"This function takes in 2 arguments");
        try
        {
            return formatNumber(stold(args[0]) / stold(args[1]));
        }
        catch (...)
        {
            throw std::wstring(L"Could not convert variables to numbers");
        }
    });
}

int main(int argc, char* argv[])
{
    ConsoleController console;
    if (argc < 2)
    {
        console.showFatal("No file provided");
        return 0;
    }

    Storage builtins;
    registerBuiltins(builtins);
    
    PitonRuntime runtime;
    runtime.addFuntions(builtins);
    runtime.setController(console);

    try
    {
        std::wstring content = ReadFileToWString(argv[1]);
        std::wstringstream file(content);
        runtime.setInput(file);
        runtime.run(argc, argv);
    }
    catch (std::runtime_error error)
    {
        console.showFatal(error.what());
    }
    catch (std::wstring error)
    {
        console.showFatal(error);
    }
    catch (int errorCode)
    {
        console.showFatal("Error code: " + std::to_string(errorCode));
    }
    catch (...)
    {
        console.showFatal("Unknown error");
    }

    return 0;
}
