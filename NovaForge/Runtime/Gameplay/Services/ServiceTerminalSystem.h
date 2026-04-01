#pragma once

#include "Gameplay/Services/ServiceTerminalTypes.h"
#include <string>
#include <vector>

class ServiceTerminalSystem
{
public:
    bool Initialize();
    void RegisterTerminal(const ServiceTerminalState& Terminal);
    bool ActivateOption(const std::string& TerminalId, const std::string& OptionId);
    void Tick(float DeltaTime);

private:
    std::vector<ServiceTerminalState> Terminals;
};
