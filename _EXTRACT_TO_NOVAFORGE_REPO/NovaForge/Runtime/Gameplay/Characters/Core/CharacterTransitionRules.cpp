#include "Gameplay/Characters/Core/CharacterTransitionRules.h"

bool CharacterTransitionRules::CanTransition(ECharacterMovementMode FromMode,
                                             ECharacterMovementMode ToMode,
                                             const CharacterTransitionContext& Context)
{
    if (FromMode == ToMode)
    {
        return true;
    }

    if (FromMode == ECharacterMovementMode::FPS && ToMode == ECharacterMovementMode::EVA)
    {
        return Context.bInAirlockExitZone || Context.bZeroG;
    }

    if (FromMode == ECharacterMovementMode::EVA && ToMode == ECharacterMovementMode::FPS)
    {
        return Context.bInPressurizedInterior;
    }

    if (FromMode == ECharacterMovementMode::FPS && ToMode == ECharacterMovementMode::Mech)
    {
        return Context.bNearMechCockpit;
    }

    if (FromMode == ECharacterMovementMode::Mech && ToMode == ECharacterMovementMode::FPS)
    {
        return Context.bInPressurizedInterior || Context.bNearMechCockpit;
    }

    return false;
}
