#include "console.h"
#include "runtime.h"

int main(int argc, char* argv[])
{
    ConsoleController console;
    if (argc < 2)
    {
        console.showFatal("No file provided");
        return 0;
    }
    
    PitonRuntime runtime;
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
        throw;
    }

    return 0;
}
