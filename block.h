#pragma once

#include "storage.h"

#include <sstream>
#include <string>
#include <vector>

class Block
{
public:
	void exec(Storage& functions);
	void addLine(std::wstring& line);
private:
	std::vector<std::wstring> line;
};