#include "runtime.h"

void PitonRuntime::setInput(std::wistream& input)
{
	this->input = &input;
}
void PitonRuntime::setController(ConsoleController& controller)
{
	this->controller = controller;
}
void PitonRuntime::addFuntions(Storage& functions)
{
	this->functions.merge(functions);
}

void PitonRuntime::run(int argc, char* argv[])
{
    if (!this->input) return;

    Block block;

    try
    {
        for (int i = 0; i < argc; i++)
            block.define(L"argv" + std::to_wstring(i));
        int line = 1;
        block.build([this, &line]() -> wchar_t {
            wchar_t c;
            if (this->input->get(c))
            {
                if (c == L'\n') line++;
                return c;
            }
            return WEOF;
        }, [&line]() -> int { return line; });

        std::vector<std::wstring> args;
        args.reserve(argc);
        for (int i = 0; i < argc; ++i)
        {
            std::string arg = argv[i];
            args.emplace_back(arg.begin(), arg.end());
        }
        block.prepare(this->functions);
        block.exec(args, nullptr);
    }
    catch (const std::runtime_error& error)
    {
        this->controller.showError(std::string(error.what()) + '\n');
    }
    catch (const std::wstring error)
    {
        this->controller.showError(L"[Runtime]: " + error + L'\n');
    }
}