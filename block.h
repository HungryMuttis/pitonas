#pragma once
#include "storage.h"
#include "common.h"

#include <sstream>
#include <string>
#include <functional>
#include <vector>

struct Token
{
	bool literal = false;
	std::wstring token;
};

class Scope
{
public:
	Scope(Scope* parent = nullptr);
	void set(const std::wstring& name, const std::wstring& val);
	std::wstring getv(const std::wstring& name);
	
	void add(const std::wstring& name, Function func);
	Function getf(const std::wstring& name);
private:
	std::map<std::wstring, std::wstring> variables;
	Storage functions;
	Scope* parent;
};

enum class BlockType
{
	Global, // disconnected scope
	Local,  // scope linked to a parent
	Shared  // same scope
};

class Block;

struct FunctionEntry
{
	std::wstring name;
	BlockType type;
	std::shared_ptr<Block> block;
};

typedef std::function<void(Scope& scope, Storage& globals, std::wstring& returnVal, int& execLine)> Line;
typedef std::function<std::wstring(Scope& scope, Storage& globals, const std::vector<Token>& args)> FunctionExecute;

class Block
{
public:
	void define(const std::wstring& name);
	void setType(BlockType type);
	void prepare(Storage& globals);
	std::wstring exec(const std::vector<std::wstring>& args, Scope* parentScope);
	void build(const std::function<wchar_t()>& getChar, const std::function<int()>& getLine);
private:
	void addToken(std::wstringstream& currToken, std::vector<Token>& tokens, bool literal = false);
	FunctionExecute functionExecute(std::wstring& name) const noexcept;
	std::vector<Line> lines;
	std::vector<std::wstring> parameters;
	std::vector<std::shared_ptr<Block>> children;
	std::vector<FunctionEntry> functions;

	bool ready = false, returned = false;

	BlockType type = BlockType::Global;
	Storage* globals = nullptr;
};