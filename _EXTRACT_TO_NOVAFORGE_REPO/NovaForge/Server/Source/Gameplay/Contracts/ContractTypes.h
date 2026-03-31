#pragma once

#include <string>
#include <vector>

enum class EContractType
{
    Salvage,
    Mining,
    Delivery,
    Repair
};

struct ContractRequirement
{
    std::string TargetId;
    int Count = 0;
};

struct ContractDefinition
{
    std::string ContractId;
    EContractType Type = EContractType::Salvage;
    std::string FactionId;
    int CreditReward = 0;
    int ReputationReward = 0;
    int RequiredStanding = 0;
    std::vector<ContractRequirement> Requirements;
};

struct ActiveContractState
{
    std::string ContractId;
    bool bAccepted = false;
    bool bCompleted = false;
    int Progress = 0;
};
