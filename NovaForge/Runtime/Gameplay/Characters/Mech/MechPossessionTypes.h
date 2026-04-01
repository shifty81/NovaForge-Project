#pragma once

#include <string>

struct MechPossessionState
{
    std::string CharacterId;
    std::string MechId;
    bool bPossessing = false;
};
