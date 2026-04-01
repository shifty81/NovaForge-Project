// BuilderSystem.cpp
#include "BuilderSystem.h"
#include <sstream>

namespace NovaForge::Gameplay::Builder
{

void BuilderSystem::initialise() {}
void BuilderSystem::shutdown()   {}

uint64_t BuilderSystem::createBlueprint(const std::string& name, uint32_t socketCount)
{
    ConstructionBlueprint bp;
    bp.blueprintId = nextBlueprintId_++;
    bp.name        = name;
    for (uint32_t i = 0; i < socketCount; ++i)
    {
        BuildSocket s;
        std::ostringstream ss; ss << "socket-" << (i + 1);
        s.socketId   = ss.str();
        s.socketType = "generic";
        bp.sockets.push_back(s);
    }
    blueprints_.push_back(bp);
    return bp.blueprintId;
}

bool BuilderSystem::attachModule(uint64_t bpId, const std::string& socketId,
                                 const BuildingModule& mod)
{
    for (auto& bp : blueprints_)
    {
        if (bp.blueprintId != bpId) continue;
        for (auto& s : bp.sockets)
        {
            if (s.socketId != socketId || s.occupied) continue;
            s.occupied   = true;
            s.occupantId = mod.moduleId;
            bp.validated = false;
            return true;
        }
    }
    return false;
}

bool BuilderSystem::detachModule(uint64_t bpId, const std::string& socketId)
{
    for (auto& bp : blueprints_)
    {
        if (bp.blueprintId != bpId) continue;
        for (auto& s : bp.sockets)
        {
            if (s.socketId != socketId || !s.occupied) continue;
            s.occupied   = false;
            s.occupantId.clear();
            bp.validated = false;
            return true;
        }
    }
    return false;
}

ValidationResult BuilderSystem::validate(uint64_t bpId) const
{
    ValidationResult r;
    for (const auto& bp : blueprints_)
    {
        if (bp.blueprintId != bpId) continue;
        // Stub: blueprint is valid as long as at least one socket is occupied
        bool anyOccupied = false;
        for (const auto& s : bp.sockets)
            if (s.occupied) { anyOccupied = true; break; }
        r.valid = anyOccupied;
        if (!anyOccupied) r.issues.push_back("No modules attached");
        return r;
    }
    r.issues.push_back("Blueprint not found");
    return r;
}

bool BuilderSystem::commit(uint64_t bpId)
{
    for (auto& bp : blueprints_)
    {
        if (bp.blueprintId != bpId) continue;
        auto v = validate(bpId);
        if (!v.valid) return false;
        bp.validated = true;
        return true;
    }
    return false;
}

} // namespace NovaForge::Gameplay::Builder
