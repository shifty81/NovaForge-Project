#pragma once

#include "LegacyAdapters/Core/LegacyAdapterTypes.h"
#include <string>

class LegacySourceClassifier
{
public:
    static LegacySourceInfo Classify(const std::string& SourcePath);
};
