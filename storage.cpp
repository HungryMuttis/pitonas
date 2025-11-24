#include "storage.h"

void Storage::add(std::wstring name, Function func)
{
	this->functions[name] = func;
}
std::function<std::wstring(std::vector<std::wstring>)> Storage::resolve(const std::wstring name)
{
    auto iterator = this->functions.find(name);
    if (iterator == this->functions.end()) throw std::wstring(L"No function with name '" + name + L"' was found");

    return iterator->second;
}
std::wstring Storage::call(std::wstring name, std::vector<std::wstring> args)
{
    return this->resolve(name)(args);
}
void Storage::merge(const Storage& other)
{
	this->functions.insert(other.functions.begin(), other.functions.end());
}
Storage operator+(Storage copy, const Storage& other)
{
	copy.merge(other);
	return copy;
}