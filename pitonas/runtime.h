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
	void run(int argv, char* argc[]);
private:
	std::wistream* input = nullptr;
	ConsoleController controller;
};