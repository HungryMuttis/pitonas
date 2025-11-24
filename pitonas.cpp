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
        for (std::wstring string : args)
            number += getNumber(string);
        return formatNumber(number);
    });
    builtins.add(L"atimk", [](std::vector<std::wstring> args) {
        if (args.size() < 2) throw std::wstring(L"This function takes in atleast 2 arguments");
        long double number = getNumber(args[0]);
        for (int i = 1; i < args.size(); i++)
            number -= getNumber(args[i]);
        return formatNumber(number);
    });
    builtins.add(L"padaugink", [](std::vector<std::wstring> args) {
        if (args.size() < 2) throw std::wstring(L"This function takes in atleats 2 arguments");
        long double number = getNumber(args[0]);
        for (int i = 1; i < args.size(); i++)
            number *= getNumber(args[i]);
        return formatNumber(number);
    });
    builtins.add(L"padalink", [](std::vector<std::wstring> args) {
        if (args.size() < 2) throw std::wstring(L"This function takes in atleast 2 arguments");
        long double number = getNumber(args[0]);
        for (int i = 1; i < args.size(); i++)
            number /= getNumber(args[i]);
        return formatNumber(number);
    });

    // boolean
    builtins.add(L"daugiau", [](std::vector<std::wstring> args) {
        if (args.size() != 2) throw std::wstring(L"This function takes in 2 arguments");
        return getNumber(args[0]) > getNumber(args[1]) ? L"taip" : L"ne";
    });
    builtins.add(L"mažiau", [](std::vector<std::wstring> args) {
        if (args.size() != 2) throw std::wstring(L"This function takes in 2 arguments");
        return getNumber(args[0]) < getNumber(args[1]) ? L"taip" : L"ne";
    });
    builtins.add(L"lygu", [](std::vector<std::wstring> args) {
        if (args.size() != 2) throw std::wstring(L"This function takes in 2 arguments");
        return args[0] == args[1] ? L"taip" : L"ne";
    });
    builtins.add(L"nelygu", [](std::vector<std::wstring> args) {
        if (args.size() != 2) throw std::wstring(L"This function takes in 2 arguments");
        return args[0] != args[1] ? L"taip" : L"ne";
    });

    builtins.add(L"ir", [](std::vector<std::wstring> args) {
        if (args.size() < 2) throw std::wstring(L"This function takes in at least 2 arguments");
        for (std::wstring string : args)
            if (!getBool(string)) return L"ne";
        return L"taip";
    });
    builtins.add(L"arba", [](std::vector<std::wstring> args) {
        if (args.size() < 2) throw std::wstring(L"This function takes in at least 2 arguments");
        for (std::wstring string : args)
            if (getBool(string)) return L"taip";
        return L"ne";
    });
    builtins.add(L"atvirkščiai", [](std::vector<std::wstring> args) {
        if (args.size() != 1) throw std::wstring(L"This function takes in 1 argument");
        return getBool(args[0]) ? L"ne" : L"taip";
    });

    builtins.add(L"yra_skaičius", [](std::vector<std::wstring> args) {
        if (args.size() != 1) throw std::wstring(L"This function takes in 1");
        try
        {
            getNumber(args[0]);
            return L"taip";
        }
        catch (const std::wstring&)
        {
            return L"ne";
        }
    });
    builtins.add(L"yra_loginis", [](std::vector<std::wstring> args) {
        if (args.size() != 1) throw std::wstring(L"This function takes in 1 argument");
        try
        {
            getBool(args[0]);
            return L"taip";
        }
        catch (const std::wstring&)
        {
            return L"ne";
        }
    });

    // system interaction
    //builtins.add(L"laikas", [](std::vector<std::wstring> args) {

    //});
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
