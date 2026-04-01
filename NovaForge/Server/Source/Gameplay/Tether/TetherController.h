#pragma once

#include "Gameplay/Tether/TetherTypes.h"

class TetherController
{
public:
    bool Initialize(float InMaxLength);
    void Activate();
    void Deactivate();
    void SetLength(float NewLength);
    void Tick(float DeltaTime);

    const TetherState& GetState() const;

private:
    TetherState State;
};
