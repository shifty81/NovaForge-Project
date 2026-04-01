#include "Gameplay/Services/ServiceTerminalSystem.h"
#include <iostream>

bool ServiceTerminalSystem::Initialize()
{
    std::cout << "[Services] Initialize\n";
    return true;
}

void ServiceTerminalSystem::RegisterTerminal(const ServiceTerminalState& Terminal)
{
    Terminals.push_back(Terminal);
    std::cout << "[Services] Registered terminal " << Terminal.TerminalId << "\n";
}

bool ServiceTerminalSystem::ActivateOption(const std::string& TerminalId, const std::string& OptionId)
{
    for (const auto& Terminal : Terminals)
    {
        if (Terminal.TerminalId == TerminalId)
        {
            for (const auto& Option : Terminal.Options)
            {
                if (Option.OptionId == OptionId && Option.bEnabled)
                {
                    std::cout << "[Services] Activated " << OptionId << " on " << TerminalId << "\n";
                    return true;
                }
            }
        }
    }
    return false;
}

void ServiceTerminalSystem::Tick(float)
{
    std::cout << "[Services] Terminals=" << Terminals.size() << "\n";
}
