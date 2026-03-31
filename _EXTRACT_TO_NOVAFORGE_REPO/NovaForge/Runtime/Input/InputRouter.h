#pragma once

#include "Input/InputTypes.h"
#include <string>
#include <vector>

class InputRouter
{
public:
    bool Initialize(const InputConfig& InConfig);
    InputFrameState BuildFrameState(const std::vector<std::string>& PressedKeys, float LookYawDelta, float LookPitchDelta) const;

private:
    InputConfig Config;
};
