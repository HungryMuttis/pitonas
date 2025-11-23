#include "common.h"
#include "console.h"
#include "runtime.h"

static void registerBuiltins(Storage& builtins)
{
    // printing
    builtins.add(L"atspausdink", [](std::vector<std::wstring> args) {
        for (std::wstring string : args)
            std::wcout << string << L' ';
        std::wcout << std::endl;
        return L"";
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
        std::wifstream file(argv[1]);
        if (!file.is_open()) throw std::wstring(L"Failed to open file '" + ToWString(argv[1]) + L'\'');
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
