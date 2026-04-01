#pragma once

#include <string>
#include <vector>

enum class ELegacyOrigin
{
    Unknown,
    NovaForge,
    Arbiter,
    ArbiterAI,
    MasterRepo
};

enum class EAdapterDecision
{
    Adopt,
    Refactor,
    ReferenceOnly,
    CompareFirst,
    ArchiveOnly,
    Discard
};

struct LegacySourceInfo
{
    ELegacyOrigin Origin = ELegacyOrigin::Unknown;
    std::string SourcePath;
    std::string SourceSystem;
    EAdapterDecision Decision = EAdapterDecision::CompareFirst;
};

struct AdapterReport
{
    bool bSuccess = false;
    std::string Summary;
    std::vector<std::string> Warnings;
};
