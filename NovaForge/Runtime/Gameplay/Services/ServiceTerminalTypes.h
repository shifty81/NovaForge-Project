#pragma once

#include <string>
#include <vector>

struct ServiceTerminalOption
{
    std::string OptionId;
    std::string Label;
    bool bEnabled = true;
};

struct ServiceTerminalState
{
    std::string TerminalId;
    std::vector<ServiceTerminalOption> Options;
};
