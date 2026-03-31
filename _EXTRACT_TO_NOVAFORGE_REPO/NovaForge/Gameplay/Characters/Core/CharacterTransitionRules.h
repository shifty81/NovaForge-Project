#pragma once

#include "Characters/CharacterTypes.h"

struct CharacterTransitionContext
{
    bool bInAirlockExitZone = false;
    bool bInPressurizedInterior = true;
    bool bNearMechCockpit = false;
    bool bZeroG = false;
};

class CharacterTransitionRules
{
public:
    static bool CanTransition(ECharacterMovementMode FromMode,
                              ECharacterMovementMode ToMode,
                              const CharacterTransitionContext& Context);
};
