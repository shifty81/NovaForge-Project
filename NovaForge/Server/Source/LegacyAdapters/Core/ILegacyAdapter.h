#pragma once

#include "LegacyAdapters/Core/LegacyAdapterTypes.h"
#include <string>

class ILegacyAdapter
{
public:
    virtual ~ILegacyAdapter() = default;

    virtual const char* GetAdapterName() const = 0;
    virtual AdapterReport ValidateSource(const LegacySourceInfo& Source) const = 0;
    virtual AdapterReport Convert(const std::string& RawInput, std::string& OutConverted) const = 0;
};
