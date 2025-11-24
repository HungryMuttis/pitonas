#include "block.h"

void Block::define(const std::wstring& name)
{
	this->parameters.push_back(name);
}

void Block::setType(BlockType type)
{
	this->type = type;
}

void Block::prepare(Storage& globals)
{
	this->globals = &globals;
	this->ready = true;

	for (std::shared_ptr<Block> block : this->children)
		block->prepare(globals);
}

std::wstring Block::exec(const std::vector<std::wstring>& args, Scope* parentScope)
{
	if (!this->ready) throw std::wstring(L"Block must first be prepared before it can be executed");
	if (args.size() > this->parameters.size()) throw std::wstring(L"Not enough parameters supplied");

	this->returned = false;

	Scope* activeScope = nullptr;
	std::unique_ptr<Scope> newScope = nullptr;

	if (this->type == BlockType::Shared)
	{
		if (!parentScope) throw std::wstring(L"Shared block must get a parent scope");
		activeScope = parentScope;
	}
	else if (this->type == BlockType::Local)
	{
		newScope = std::make_unique<Scope>(parentScope);
		activeScope = newScope.get();
	}
	else
	{
		newScope = std::make_unique<Scope>(nullptr);
		activeScope = newScope.get();
	}

	for (int i = 0; i < this->parameters.size(); i++)
		activeScope->set(this->parameters[i], args[i]);

	for (const FunctionEntry& function : this->functions)
	{
		std::shared_ptr<Block> block = function.block;
		Function func = [block](std::vector<std::wstring> args) { return block->exec(args, nullptr); };

		if (function.type == BlockType::Global) this->globals->add(function.name, func);
		else activeScope->add(function.name, func);
	}

	std::wstring returnVal = L"";

	for (int execLine = 0; execLine < this->lines.size(); execLine++)
	{
		this->lines[execLine](*activeScope, *this->globals, returnVal, execLine);
		if (execLine == -2) { this->returned = true; break; }
	}

	return returnVal;
}

void Block::build(const std::function<wchar_t()>& getChar, const std::function<int()>& getLine)
{
	bool string = false, escape = false, comment = false, lastif = false;
	Token lastiftoken;

	std::wstringstream currToken;
	std::vector<Token> tokens;

	try
	{
		while (true)
		{
			wchar_t c = getChar();
			if (c == WEOF || c == L'\0')
			{
				if (!tokens.empty()) throw std::wstring(L"File ended prematurely");
				return;
			}

			if (comment && c != L'#') continue;

			switch (c)
			{
			case L'}':
				if (!tokens.empty()) throw std::wstring(L"Block ended prematurely");
				return;
			case L'\t':
			case L'\r':
			case L'\n':
			case L' ':
				if (string) currToken << c;
				else this->addToken(currToken, tokens);
				break;
			case L'"':
				if (escape)
				{
					escape = false;
					currToken << c;
				}
				else
				{
					string = !string;
					if (string == false) this->addToken(currToken, tokens, true);
				}
				break;
			case L'\\':
				if (escape)
				{
					escape = false;
					currToken << c;
				}
				else escape = true;
				break;
			case L'{':
			{
				if (string)
				{
					currToken << c;
					break;
				}
				this->addToken(currToken, tokens);

				std::shared_ptr<Block> newBlock = std::make_shared<Block>();
				this->children.push_back(newBlock);

				if (tokens.size() < 1)
				{
					newBlock->setType(BlockType::Shared);
					newBlock->build(getChar, getLine);
					this->lines.push_back([newBlock](Scope& scope, Storage&, std::wstring&, int&) { newBlock->exec({}, &scope); });
					break;
				}
				if (tokens[0].literal) throw std::wstring(L"First token cannot be literal");

				std::wstring opcode = tokens[0].token;

				if (opcode == L"funkcija")
				{
					lastif = false;
					if (tokens.size() < 3) throw std::wstring(L"Function needs at least 3 tokens to be defined");
					if (tokens[1].literal) throw std::wstring(L"Function type cannot be literal");

					std::wstring type = tokens[1].token;

					BlockType blockType;
					if (type == L"vietinė") blockType = BlockType::Local;
					else if (type == L"globali") blockType = BlockType::Global;
					else throw std::wstring(L"Unknown function type");

					newBlock->setType(blockType);

					if (tokens[2].literal) throw std::wstring(L"Function name cannot be literal");

					for (int i = 3; i < tokens.size(); i++)
					{
						if (tokens[i].literal) throw std::wstring(L"Function parameters cannot be literal");
						newBlock->define(tokens[i].token);
					}

					newBlock->build(getChar, getLine);

					FunctionEntry function;
					function.name = tokens[2].token;
					function.type = blockType;
					function.block = newBlock;
					this->functions.push_back(function);
				}
				else if (opcode == L"jeigu")
				{
					if (tokens.size() != 2) throw std::wstring(L"If statement needs 2 parameters");

					Token arg = tokens[1];
					lastiftoken = arg;
					newBlock->setType(BlockType::Shared);
					newBlock->build(getChar, getLine);
					this->lines.push_back([newBlock, arg](Scope& scope, Storage&, std::wstring& returnVal, int& execLine) {
						std::wstring resolvedArg = arg.literal ? arg.token : scope.getv(arg.token);
						if (resolvedArg == L"taip")
						{
							std::wstring ret = newBlock->exec({}, &scope);
							if (newBlock->returned) { execLine = -2; returnVal = ret; }
						}
						else if (resolvedArg != L"ne") throw std::wstring(L"IF statement only accepts boolean arguments");
					});
					lastif = true;
				}
				else if (opcode == L"priešingai")
				{
					if (!lastif) throw std::wstring(L"ELSE block cannot be used when theres no IF block");
					lastif = false;
					if (tokens.size() != 1) throw std::wstring(L"ELSE block need only 1 token");
	
					newBlock->setType(BlockType::Shared);
					newBlock->build(getChar, getLine);
					this->lines.push_back([newBlock, lastiftoken](Scope& scope, Storage&, std::wstring& returnVal, int& execLine) {
						std::wstring resolvedArg = lastiftoken.literal ? lastiftoken.token : scope.getv(lastiftoken.token);
						if (resolvedArg == L"ne")
						{
							std::wstring ret = newBlock->exec({}, &scope);
							if (newBlock->returned) { execLine = -2; returnVal = ret; }
						}
					});
				}
				else throw std::wstring(L"Unknown first token");

				tokens.clear();

				break;
			}
			case L';':
			{
				if (string)
				{
					currToken << c;
					break;
				}
				this->addToken(currToken, tokens);
				lastif = false;

				if (tokens.empty()) break;
				if (tokens[0].literal) throw std::wstring(L"First token cannot be literal");

				Line newLine;
				std::wstring opcode = tokens[0].token;
				if (opcode == L"kilpa")
				{
					if (tokens.size() > 2) throw std::wstring(L"Too many arguments for a loop");
					std::wstring argname = L"";
					if (tokens.size() == 2)
					{
						if (tokens[1].literal) throw std::wstring(L"Jump line cannot be literal. It may be a number or a variable");
						argname = tokens[1].token;
					}
					newLine = [argname](Scope& scope, Storage&, std::wstring&, int& execLine) {
						if (argname == L"") execLine = -1;
						else try { execLine = getNumber(argname); } catch (...) { execLine = getNumber(scope.getv(argname)); }
					};
				}
				else if (opcode == L"kintamasis")
				{
					if (tokens.size() < 2) throw std::wstring(L"Variable name must be defined");
					if (tokens[1].literal) throw std::wstring(L"Variable name cannot be literal");
					std::wstring variable = tokens[1].token;
					FunctionExecute func = [](Scope&, Storage&, std::vector<Token>) { return L""; };
					if (tokens.size() > 2)
					{
						if (tokens[2].literal)
						{
							if (tokens.size() > 3) throw std::wstring(L"Variable can only hold one value at a time");
							std::wstring val = tokens[2].token;
							func = [val](Scope&, Storage&, const std::vector<Token>&) { return val; };
						}
						else
						{
							std::wstring name = tokens[2].token;
							func = this->functionExecute(name);
						}
					}
					std::vector<Token> args;
					if (tokens.size() > 3) args.assign(tokens.begin() + 3, tokens.end());
					newLine = [variable, func, args](Scope& scope, Storage& globals, std::wstring&, int&) { scope.set(variable, func(scope, globals, args)); };
				}
				else if (opcode == L"grąžink")
				{
					if (tokens.size() > 2) throw std::wstring(L"Only one return value can be specified");
					if (tokens.size() == 2)
					{
						FunctionExecute func;
						std::wstring token = tokens[1].token;
						if (tokens[1].literal) newLine = [token](Scope&, Storage&, std::wstring& returnVal, int& execLine) { returnVal = token; execLine = -2; };
						else newLine = [token](Scope& scope, Storage&, std::wstring& returnVal, int& execLine) { returnVal = scope.getv(token); execLine = -2; };
					}
					else
					{
						newLine = [](Scope&, Storage&, std::wstring&, int& execLine) { execLine = -2; };
					}
				}
				else
				{
					FunctionExecute func = this->functionExecute(opcode);
					std::vector<Token> args;
					if (tokens.size() > 1) args.assign(tokens.begin() + 1, tokens.end());
					newLine = [func, args](Scope& scope, Storage& globals, std::wstring&, int&) { func(scope, globals, args); };
				}

				int currLine = getLine();
				newLine = [newLine, currLine](Scope& scope, Storage& globals, std::wstring& returnVal, int& execLine) { try { return newLine(scope, globals, returnVal, execLine); } catch (std::wstring& error) { throw std::wstring(L"Line " + std::to_wstring(currLine) + L": " + error); } };
				this->lines.push_back(newLine);
				tokens.clear();

				break;
			}
			case L'#':
				if (escape)
				{
					escape = false;
					currToken << c;
				}
				else comment = !comment;
				break;
			case L'n':
				if (escape)
				{
					escape = false;
					currToken << '\n';
				}
				else currToken << c;
				break;
			default:
				currToken << c;
				break;
			}
		}
	}
	catch (std::wstring error)
	{
		throw std::wstring(L"[Compilation]: Line " + std::to_wstring(getLine()) + L": " + error);
	}
}

void Block::addToken(std::wstringstream& currToken, std::vector<Token>& tokens, bool literal)
{
	if (!literal && currToken.tellp() == 0) return;
	Token token;
	token.literal = literal;
	token.token = currToken.str();
	tokens.push_back(token);
	currToken.str(L"");
}

FunctionExecute Block::functionExecute(std::wstring& name) const noexcept
{
	return [name](Scope& scope, Storage& globals, const std::vector<Token>& args) { 
		std::vector<std::wstring> resolvedArgs;
		resolvedArgs.reserve(args.size());
		for (const Token& token : args)
			resolvedArgs.push_back(token.literal ? token.token : scope.getv(token.token));

		return [](std::wstring name, Scope& scope, Storage& globals) {
			try { return scope.getf(name); }
			catch (...) { return globals.resolve(name); }
		}(name, scope, globals)(resolvedArgs);
	};
}

Scope::Scope(Scope* parent) : parent(parent) {}

void Scope::set(const std::wstring& name, const std::wstring& val)
{
	this->variables[name] = val;
}
std::wstring Scope::getv(const std::wstring& name)
{
	std::map<std::wstring, std::wstring>::iterator iterator = this->variables.find(name);
	if (iterator != this->variables.end()) return iterator->second;
	if (this->parent) return this->parent->getv(name);
	throw std::wstring(L"Variable '" + name + L"' is not defined");
}

void Scope::add(const std::wstring& name, Function func)
{
	this->functions.add(name, func);
}
Function Scope::getf(const std::wstring& name)
{
	try
	{
		return this->functions.resolve(name);
	}
	catch (...)
	{
		if (this->parent) return this->parent->getf(name);
		throw;
	}
}