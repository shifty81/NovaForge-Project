#pragma once

#include "Gameplay/Titan/TitanTypes.h"
#include <string>
#include <vector>

class TitanConstructionSystem
{
public:
    bool Initialize();
    void RegisterProject(const TitanProjectState& Project);
    bool ContributeResource(const std::string& TitanId, const std::string& ResourceId, int Amount);
    bool IsProjectComplete(const std::string& TitanId) const;
    const TitanProjectState* FindProject(const std::string& TitanId) const;
    void Tick(float DeltaTime);

private:
    std::vector<TitanProjectState> Projects;
};
