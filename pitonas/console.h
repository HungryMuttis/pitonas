#pragma once
#include "common.h"

#include <fcntl.h>
#include <iostream>
#include <io.h>
#include <Windows.h>

class ConsoleController
{
public:
	ConsoleController();
	void showError(std::string message) const;
	void showError(std::wstring message) const;
	void showFatal(std::string message) const;
	void showFatal(std::wstring message) const;
	void resetColor() const;
private:
	HANDLE hConsole;
};