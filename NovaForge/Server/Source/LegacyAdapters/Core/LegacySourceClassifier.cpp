#include "LegacyAdapters/Core/LegacySourceClassifier.h"

LegacySourceInfo LegacySourceClassifier::Classify(const std::string& SourcePath)
{
    LegacySourceInfo Info;
    Info.SourcePath = SourcePath;

    if (SourcePath.find("NovaForge") != std::string::npos)
    {
        Info.Origin = ELegacyOrigin::NovaForge;
        Info.SourceSystem = "Gameplay/Data";
    }
    else if (SourcePath.find("ArbiterAI") != std::string::npos)
    {
        Info.Origin = ELegacyOrigin::ArbiterAI;
        Info.SourceSystem = "AI";
    }
    else if (SourcePath.find("Arbiter") != std::string::npos)
    {
        Info.Origin = ELegacyOrigin::Arbiter;
        Info.SourceSystem = "Tooling";
    }
    else if (SourcePath.find("MasterRepo") != std::string::npos)
    {
        Info.Origin = ELegacyOrigin::MasterRepo;
        Info.SourceSystem = "Architecture/Mixed";
    }
    else
    {
        Info.Origin = ELegacyOrigin::Unknown;
        Info.SourceSystem = "Unknown";
    }

    Info.Decision = EAdapterDecision::CompareFirst;
    return Info;
}
