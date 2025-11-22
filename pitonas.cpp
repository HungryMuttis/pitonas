#include "common.h"
#include "console.h"
#include "runtime.h"

static void registerBuiltins(Storage& builtins)
{
    // printing
    builtins.add(L"atspausdink", +[](std::vector<std::wstring> args) {
        for (std::wstring string : args)
            std::wcout << string << L' ';
        std::wcout << std::endl;
    });
}

int main(int argc, char* argv[])
{
    ConsoleController console;

    Storage builtins;
    registerBuiltins(builtins);
    
    PitonRuntime runtime;
    runtime.addFuntions(builtins);
    runtime.setController(console);

    std::wifstream file;
    try
    {
        if (argc >= 2)
        {
            file.open(argv[1]);
            runtime.setInput(file);
        }
        else runtime.usingConsole();
        runtime.run();
    }
    catch (std::runtime_error error)
    {
        console.showFatal(error.what());
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
