#pragma once

#include "LegacyAdapters/Core/ILegacyAdapter.h"

class LegacyDataAdapter : public ILegacyAdapter
{
public:
    const char* GetAdapterName() const override;
    AdapterReport ValidateSource(const LegacySourceInfo& Source) const override;
    AdapterReport Convert(const std::string& RawInput, std::string& OutConverted) const override;
};
