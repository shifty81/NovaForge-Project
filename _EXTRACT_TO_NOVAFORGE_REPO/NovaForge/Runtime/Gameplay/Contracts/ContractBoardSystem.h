#pragma once

#include "Gameplay/Contracts/ContractTypes.h"
#include <string>
#include <vector>

class ContractBoardSystem
{
public:
    bool Initialize();
    void OfferContract(const ContractDefinition& Contract);
    bool AcceptContract(const std::string& ContractId, int CurrentStanding);
    void AdvanceProgress(const std::string& ContractId, int Delta);
    bool CompleteContract(const std::string& ContractId);
    const std::vector<ContractDefinition>& GetOfferedContracts() const;
    const std::vector<ActiveContractState>& GetActiveContracts() const;
    void Tick(float DeltaTime);

private:
    std::vector<ContractDefinition> OfferedContracts;
    std::vector<ActiveContractState> ActiveContracts;
};
