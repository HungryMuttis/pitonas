#pragma once
#include <any>
#include <functional>
#include <iostream>
#include <map>
#include <typeindex>
#include <vector>

class Storage
{
public:
    void add(std::wstring name, std::function<std::wstring(std::vector<std::wstring>)> func);
    std::function<std::wstring(std::vector<std::wstring>)> resolve(const std::wstring name);
    std::wstring call(std::wstring name, std::vector<std::wstring> args);
    void merge(const Storage& other);
    friend Storage operator+(Storage copy, const Storage& other);
private:
    std::map<std::wstring, std::function<std::wstring(std::vector<std::wstring>)>> functions;
};