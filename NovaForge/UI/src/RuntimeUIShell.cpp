#include "RuntimeUIShell.h"
#include <iostream>

bool RuntimeUIShell::Initialize()
{
    std::cout << "[RuntimeUI] Initialize\n";
    return true;
}

void RuntimeUIShell::Tick(float)
{
    std::cout << "[RuntimeUI] Tick\n";
    for (const auto& Message : Messages)
    {
        std::cout << "  UI> " << Message << "\n";
    }
    Messages.clear();
}

void RuntimeUIShell::Shutdown()
{
    std::cout << "[RuntimeUI] Shutdown\n";
}

void RuntimeUIShell::PushMessage(const std::string& Message)
{
    Messages.push_back(Message);
}
