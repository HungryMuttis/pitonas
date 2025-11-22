#pragma once
#include <any>
#include <iostream>
#include <map>
#include <typeindex>
#include <vector>

class Storage
{
public:
    template<typename Ret>
    void add(std::wstring name, Ret(*func)(std::vector<std::wstring>))
    {
        this->functions[name] = func;
    }
    template<typename Ret = void> Ret call(std::wstring name, std::vector<std::wstring> args)
    {
        std::map<std::wstring, std::any>::iterator iterator = this->functions.find(name);
        if (iterator == this->functions.end()) throw std::wstring(L"No function with name '" + name + L"' was found");

        return std::any_cast<Ret(*)(std::vector<std::wstring>)>(iterator->second)(args);
    }
    void merge(const Storage& other);
    friend Storage operator+(Storage copy, const Storage& other);
private:
    std::map<std::wstring, std::any> functions;
};