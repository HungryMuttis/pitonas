#include "storage.h"

void Storage::merge(const Storage& other)
{
	for (const auto& [name, function] : other.functions)
		this->functions[name] = function;
}
Storage operator+(Storage copy, const Storage& other)
{
	copy.merge(other);
	return copy;
}