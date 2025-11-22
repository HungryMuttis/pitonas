#pragma once
#include "console.h"
#include "storage.h"
#include "block.h"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <stack>

class PitonRuntime
{
public:
	void setInput(std::wistream& input);
	void setController(ConsoleController& controller);
	void addFuntions(Storage& functions);
	void usingConsole(bool setting = true);
	void run();
private:
	std::wistream* input = nullptr;
	Storage functions;
	ConsoleController controller;
	bool console = false;
};