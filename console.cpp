#include "console.h"

ConsoleController::ConsoleController()
{
	auto _ = _setmode(_fileno(stdout), _O_U16TEXT);
	this->hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	this->resetColor();
}
void ConsoleController::showError(std::string message) const
{
	this->showError(ToWString(message));
}
void ConsoleController::showError(std::wstring message) const
{
	SetConsoleTextAttribute(this->hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
	std::wcout << L"[Error]: " << message;
	this->resetColor();
}
void ConsoleController::showFatal(std::string message) const
{
	this->showFatal(ToWString(message));
}
void ConsoleController::showFatal(std::wstring message) const
{
	SetConsoleTextAttribute(this->hConsole, FOREGROUND_RED);
	std::wcout << L"[Fatal Error]: " << message;
	this->resetColor();
}

void ConsoleController::resetColor() const
{
	SetConsoleTextAttribute(this->hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
}
