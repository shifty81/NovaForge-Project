#include "Gameplay/Titan/TitanConstructionSystem.h"
#include <iostream>

bool TitanConstructionSystem::Initialize()
{
    std::cout << "[Titan] Initialize\n";
    return true;
}

void TitanConstructionSystem::RegisterProject(const TitanProjectState& Project)
{
    Projects.push_back(Project);
    std::cout << "[Titan] Registered project " << Project.TitanId << "\n";
}

bool TitanConstructionSystem::ContributeResource(const std::string& TitanId, const std::string& ResourceId, int Amount)
{
    for (auto& Project : Projects)
    {
        if (Project.TitanId == TitanId)
        {
            Project.bConstructionStarted = true;

            for (auto& Requirement : Project.Requirements)
            {
                if (Requirement.ResourceId == ResourceId)
                {
                    Requirement.CurrentAmount += Amount;
                    if (Requirement.CurrentAmount > Requirement.RequiredAmount)
                    {
                        Requirement.CurrentAmount = Requirement.RequiredAmount;
                    }

                    bool bComplete = true;
                    for (const auto& Req : Project.Requirements)
                    {
                        if (Req.CurrentAmount < Req.RequiredAmount)
                        {
                            bComplete = false;
                            break;
                        }
                    }

                    Project.bConstructionComplete = bComplete;

                    std::cout << "[Titan] Contributed " << ResourceId << " x" << Amount
                              << " to " << TitanId << "\n";
                    return true;
                }
            }
        }
    }

    return false;
}

bool TitanConstructionSystem::IsProjectComplete(const std::string& TitanId) const
{
    for (const auto& Project : Projects)
    {
        if (Project.TitanId == TitanId)
        {
            return Project.bConstructionComplete;
        }
    }
    return false;
}

const TitanProjectState* TitanConstructionSystem::FindProject(const std::string& TitanId) const
{
    for (const auto& Project : Projects)
    {
        if (Project.TitanId == TitanId)
        {
            return &Project;
        }
    }
    return nullptr;
}

void TitanConstructionSystem::Tick(float)
{
    std::cout << "[Titan] Projects=" << Projects.size() << "\n";
}
