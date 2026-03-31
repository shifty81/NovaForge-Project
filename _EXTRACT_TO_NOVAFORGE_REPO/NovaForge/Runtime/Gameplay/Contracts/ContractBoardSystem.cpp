#include "Gameplay/Contracts/ContractBoardSystem.h"
#include <iostream>

bool ContractBoardSystem::Initialize()
{
    std::cout << "[Contracts] Initialize\n";
    return true;
}

void ContractBoardSystem::OfferContract(const ContractDefinition& Contract)
{
    OfferedContracts.push_back(Contract);
    std::cout << "[Contracts] Offered " << Contract.ContractId << "\n";
}

bool ContractBoardSystem::AcceptContract(const std::string& ContractId, int CurrentStanding)
{
    for (const auto& Contract : OfferedContracts)
    {
        if (Contract.ContractId == ContractId)
        {
            if (CurrentStanding < Contract.RequiredStanding)
            {
                std::cout << "[Contracts] Standing too low for " << ContractId << "\n";
                return false;
            }

            ActiveContracts.push_back({ContractId, true, false, 0});
            std::cout << "[Contracts] Accepted " << ContractId << "\n";
            return true;
        }
    }
    return false;
}

void ContractBoardSystem::AdvanceProgress(const std::string& ContractId, int Delta)
{
    for (auto& Contract : ActiveContracts)
    {
        if (Contract.ContractId == ContractId && Contract.bAccepted && !Contract.bCompleted)
        {
            Contract.Progress += Delta;
            std::cout << "[Contracts] Progress " << ContractId << " -> " << Contract.Progress << "\n";
            return;
        }
    }
}

bool ContractBoardSystem::CompleteContract(const std::string& ContractId)
{
    for (auto& Contract : ActiveContracts)
    {
        if (Contract.ContractId == ContractId && Contract.bAccepted && !Contract.bCompleted)
        {
            Contract.bCompleted = true;
            std::cout << "[Contracts] Completed " << ContractId << "\n";
            return true;
        }
    }
    return false;
}

const std::vector<ContractDefinition>& ContractBoardSystem::GetOfferedContracts() const
{
    return OfferedContracts;
}

const std::vector<ActiveContractState>& ContractBoardSystem::GetActiveContracts() const
{
    return ActiveContracts;
}

void ContractBoardSystem::Tick(float)
{
    std::cout << "[Contracts] Offered=" << OfferedContracts.size()
              << " Active=" << ActiveContracts.size() << "\n";
}
