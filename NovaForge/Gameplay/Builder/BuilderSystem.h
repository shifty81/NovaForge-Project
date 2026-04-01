// BuilderSystem.h
// NovaForge modular construction system — blueprint assembly, placement, and validation.

#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace NovaForge::Gameplay::Builder
{

struct BuildingModule
{
    std::string moduleId;
    std::string displayName;
    float       mass        = 0.0f;
    std::string socketType; ///< e.g., "core", "wing", "turret", "cargo"
};

struct BuildSocket
{
    std::string socketId;
    std::string socketType;
    bool        occupied    = false;
    std::string occupantId;
};

struct ConstructionBlueprint
{
    uint64_t               blueprintId = 0;
    std::string            name;
    std::vector<BuildSocket> sockets;
    bool                   validated   = false;
};

struct ValidationResult
{
    bool   valid       = false;
    std::vector<std::string> issues;
};

class BuilderSystem
{
public:
    BuilderSystem()  = default;
    ~BuilderSystem() = default;

    void initialise();
    void shutdown();

    uint64_t createBlueprint(const std::string& name, uint32_t socketCount);
    bool attachModule(uint64_t blueprintId, const std::string& socketId, const BuildingModule& mod);
    bool detachModule(uint64_t blueprintId, const std::string& socketId);
    ValidationResult validate(uint64_t blueprintId) const;
    bool commit(uint64_t blueprintId);

private:
    std::vector<ConstructionBlueprint> blueprints_;
    uint64_t nextBlueprintId_ = 1;
};

} // namespace NovaForge::Gameplay::Builder
