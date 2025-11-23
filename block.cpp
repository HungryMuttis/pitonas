#include "block.h"

void Block::define(std::wstring name)
{
	this->parameters.push_back(name);
}

void Block::prepare(Storage& globals)
{
	this->globals = globals;
	this->ready = true;

	for (std::shared_ptr<Block> block : this->children)
		block->prepare(globals);
}

std::wstring Block::exec(const std::vector<std::wstring>& args)
{
	if (!this->ready) throw std::wstring(L"Block must first be prepared before it can be executed");
	if (args.size() > this->parameters.size()) throw std::wstring(L"Not enough parameters supplied");

	for (int i = 0; i < this->parameters.size(); i++)
		this->variables[this->parameters[i]] = args[i];

	for (int execLine = 0; execLine < this->lines.size(); execLine++)
		this->lines[execLine](this->variables, this->locals, this->globals, execLine);

	return std::wstring();
}
std::wstring Block::operator()(const std::vector<std::wstring>& args)
{
	return this->exec(args);
}

void Block::build(const std::function<wchar_t()>& getChar)
{
	bool string = false, escape = false, comment = false;

	while (true)
	{
		wchar_t c = getChar();
		if (c == WEOF || c == L'\0')
		{
			if (!this->tokens.empty()) throw std::wstring(L"File ended prematurely");
			return;
		}

		if (comment && c != L'#') continue;

		switch (c)
		{
		case L'}':
			if (!this->tokens.empty()) throw std::wstring(L"Block ended prematurely");
			return;
		case L'\t':
		case L'\n':
		case L' ':
			if (string) this->currToken << c;
			else this->addToken();
			break;
		case L'"':
			if (escape)
			{
				escape = false;
				this->currToken << c;
			}
			else
			{
				string = !string;
				if (string == false) this->addToken(true);
			}
			break;
		case L'\\':
			if (escape)
			{
				escape = false;
				this->currToken << c;
			}
			else escape = true;
			break;
		case L'{':
		{
			if (this->tokens.size() < 1) throw std::wstring(L"No function definition found");
			if (this->tokens[0].literal == true) throw std::wstring(L"Function name cannot be literal");

			std::shared_ptr<Block> newBlock = std::make_shared<Block>();
			this->children.push_back(newBlock);

			for (int i = 1; i < this->tokens.size(); i++)
			{
				if (this->tokens[i].literal) throw std::wstring(L"Function parameters cannot be literal");
				newBlock->define(this->tokens[i].token);
			}
			newBlock->build(getChar);
			this->locals.add(this->tokens[0].token, [newBlock](std::vector<std::wstring> args) { return newBlock->exec(args); });
			this->tokens.clear();

			break;
		}
		case L';':
		{
			if (string)
			{
				this->currToken << c;
				break;
			}
			this->addToken();

			if (this->tokens.empty()) break;
			if (this->tokens[0].literal) throw std::wstring(L"First token cannot be literal");

			Line newLine;
			std::wstring opcode = this->tokens[0].token;
			if (opcode == L"kilpa")
			{
				if (this->tokens.size() > 2) throw std::wstring(L"Too many arguments for a loop");
				std::wstring argname = L"";
				if (this->tokens.size() == 2)
				{
					if (this->tokens[1].literal) throw std::wstring(L"Jump line cannot be literal. It may be a number or a variable");
					argname = this->tokens[1].token;
				}
				newLine = [argname](std::map<std::wstring, std::wstring>& variables, Storage&, Storage&, int& execLine) { if (argname == L"") execLine = -1; else try { execLine = stoi(argname); } catch (...) { execLine = stoi(variables[argname]); } };
			}
			else if (opcode == L"kintamasis")
			{
				if (this->tokens.size() < 2) throw std::wstring(L"Variable name must be defined");
				if (this->tokens[1].literal) throw std::wstring(L"Variable name cannot be literal");
				std::wstring variable = this->tokens[1].token;
				std::function<std::wstring(Storage&, Storage&, std::vector<std::wstring>&)> initialValue = [](Storage&, Storage&, std::vector<std::wstring>&) { return L""; };
				if (this->tokens.size() > 2)
				{
					if (this->tokens[2].literal)
					{
						if (this->tokens.size() > 3) throw std::wstring(L"Variable can only hold one value at a time");
						std::wstring val = this->tokens[2].token;
						initialValue = [val](Storage&, Storage&, std::vector<std::wstring>&) { return val; };
					}
					else
					{
						std::wstring name = this->tokens[2].token;
						initialValue = this->functionExecute(name);
					}
				}
				std::vector<Token> args;
				if (this->tokens.size() > 3) args.assign(this->tokens.begin() + 3, this->tokens.end());
				newLine = [variable, initialValue, args](std::map<std::wstring, std::wstring>& variables, Storage& locals, Storage& globals, int&) { std::vector<std::wstring> resolvedArgs; for (Token token : args) resolvedArgs.push_back(token.literal ? token.token : variables[token.token]); variables[variable] = initialValue(locals, globals, resolvedArgs); };
			}
			else
			{
				std::function<std::wstring(Storage&, Storage&, std::vector<std::wstring>&)> func = this->functionExecute(opcode);
				std::vector<Token> args;
				if (this->tokens.size() > 1) args.assign(this->tokens.begin() + 1, this->tokens.end());
				newLine = [func, args](std::map<std::wstring, std::wstring>& variables, Storage& locals, Storage& globals, int&) { std::vector<std::wstring> resolvedArgs; for (Token token : args) resolvedArgs.push_back(token.literal ? token.token : variables[token.token]); func(locals, globals, resolvedArgs); };
			}

			this->lines.push_back(newLine);
			this->tokens.clear();

			break;
		}
		case L'#':
			if (escape)
			{
				escape = false;
				this->currToken << c;
			}
			else comment = !comment;
			break;
		case L'n':
			if (escape)
			{
				escape = false;
				this->currToken << '\n';
			}
			else this->currToken << c;
			break;
		default:
			this->currToken << c;
			break;
		}
	}
}

void Block::addToken(bool literal)
{
	if (this->currToken.tellp() == 0) return;
	Token token;
	token.literal = literal;
	token.token = this->currToken.str();
	this->tokens.push_back(token);
	this->currToken.str(L"");
}

std::function<std::wstring(Storage&, Storage&, std::vector<std::wstring>&)> Block::functionExecute(std::wstring& name)
{
	return [name](Storage& locals, Storage& globals, std::vector<std::wstring>& args) { return [](std::wstring name, Storage& locals, Storage& globals) { try { return locals.resolve(name); } catch (...) { return globals.resolve(name); } }(name, locals, globals)(args); };
}
