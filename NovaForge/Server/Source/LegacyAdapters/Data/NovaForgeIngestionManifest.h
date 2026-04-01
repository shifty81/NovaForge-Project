#pragma once

#include <string>
#include <vector>

struct IngestedRecordInfo
{
    std::string Category;
    std::string Id;
    std::string SourceTag;
};

class NovaForgeIngestionManifest
{
public:
    static std::vector<IngestedRecordInfo> GetSeededRecords();
};
