#include "block.h"

void Block::exec(Storage& functions)
{
	if (this->line.empty()) return;

	std::vector<std::wstring> args;
	if (this->line.size() > 1)
		args.assign(this->line.begin() + 1, this->line.end());

	functions.call(this->line[0], args);
}
void Block::addLine(std::wstring& line)
{
	std::vector<std::wstring> strings;

	bool isString = false, escape = false;
	std::wstringstream currentString;
	for (int i = 0; i < line.size(); i++)
	{
		switch (line[i])
		{
		case L' ':
			if (isString) currentString << line[i];
			else if (currentString.tellp() > 0)
			{
				strings.push_back(currentString.str());
				currentString.str(L"");
			}
			break;
		case L'"':
			if (escape)
			{
				escape = false;
				currentString << line[i];
			}
			else isString = !isString;
			break;
		case L'\\':
			if (escape)
			{
				escape = false;
				currentString << line[i];
			}
			else escape = true;
			break;
		case L'n':
			if (escape)
			{
				escape = false;
				currentString << '\n';
				break;
			}
			currentString << line[i];
			break;
		default:
			currentString << line[i];
			break;
		}
	}
	if (currentString.tellp() > 0) strings.push_back(currentString.str());

	this->line = strings;
}