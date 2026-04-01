#pragma once

#include "Gameplay/EVA/EVAState.h"

class EVAMovementController
{
public:
    bool Initialize();
    void SetActive(bool bInActive);
    void SetThrust(float Forward, float Right, float Up);
    void ToggleDampening();
    void Tick(float DeltaTime);

    const EVAState& GetState() const;

private:
    EVAState State;
};
