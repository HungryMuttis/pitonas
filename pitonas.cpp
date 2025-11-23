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
