#pragma once
#include <any>
#include <functional>
#include <iostream>
#include <map>
#include <typeindex>
#include <vector>

typedef std::function<std::wstring(std::vector<std::wstring>)> Function;

class Storage
{
public:
    void add(std::wstring name, Function func);
    Function resolve(const std::wstring name);
    std::wstring call(std::wstring name, std::vector<std::wstring> args);
    void merge(const Storage& other);
    friend Storage operator+(Storage copy, const Storage& other);
private:
    std::map<std::wstring, Function> functions;
};