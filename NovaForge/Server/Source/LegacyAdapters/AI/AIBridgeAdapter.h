#pragma once

#include <string>
#include <vector>

struct LegacyAITask
{
    std::string Prompt;
    std::string Mode;
};

struct MRArbiterTask
{
    std::string Intent;
    std::string Prompt;
    std::vector<std::string> Targets;
};

class AIBridgeAdapter
{
public:
    static MRArbiterTask Convert(const LegacyAITask& Legacy);
};
