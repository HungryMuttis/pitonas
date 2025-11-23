#pragma once
#include "storage.h"

#include <sstream>
#include <string>
#include <functional>
#include <vector>

struct Token
{
	bool literal = false;
	std::wstring token;
};
typedef std::function<void(std::map<std::wstring, std::wstring>&, Storage&, Storage&, std::wstring&, int&)> Line;

class Block
{
public:
	void define(std::wstring name);
	void prepare(Storage& globals);
	std::wstring exec(const std::vector<std::wstring>& args);
	std::wstring operator()(const std::vector<std::wstring>& args);
	void build(const std::function<wchar_t()>& getChar);
private:
	void addToken(bool literal = false);
	std::function<std::wstring(Storage&, Storage&, std::vector<std::wstring>&)> functionExecute(std::wstring& name);
	std::wstringstream currToken;
	std::vector<Line> lines;
	std::vector<Token> tokens;
	std::vector<std::wstring> parameters;
	std::map<std::wstring, std::wstring> variables;
	std::vector<std::shared_ptr<Block>> children;
	std::wstring returnVal;
	bool ready = false;
	Storage locals, globals;
};