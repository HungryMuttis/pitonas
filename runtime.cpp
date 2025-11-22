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

void PitonRuntime::usingConsole(bool setting)
{
	this->console = setting;
}

void PitonRuntime::run()
{
    if (this->console) std::wcout << L"--- Pitono konsolė ---\n";

    std::wistream* inStream = this->console ? &std::wcin : this->input;

    if (!inStream) return;

    Block block;
    std::wstring line;

    while (true)
    {
        if (this->console) std::wcout << L">>> ";
        if (!std::getline(*inStream, line)) break;
        if (line.empty()) continue;

        block.addLine(line);

        try
        {
            block.exec(this->functions);
        }
        catch (const std::runtime_error& error)
        {
            this->controller.showError(error.what() + '\n');
        }
        catch (const std::wstring error)
        {
            this->controller.showError(error + L'\n');
        }
    }
}