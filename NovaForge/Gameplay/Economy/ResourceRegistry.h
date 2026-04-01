// ResourceRegistry.h
// Registry for all NovaForge resource types (ore, gas, manufactured goods, etc.).

#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <optional>

namespace NovaForge::Gameplay::Economy
{

enum class ResourceCategory : uint8_t
{
    RawOre, ProcessedMetal, Gas, Components, Fuel, Contraband, Luxury
};

struct ResourceDefinition
{
    std::string       id;
    std::string       displayName;
    ResourceCategory  category = ResourceCategory::RawOre;
    float             baseMass = 1.0f;   // kg per unit
    float             baseVolume = 1.0f; // m³ per unit
    bool              isLegal  = true;
};

class ResourceRegistry
{
public:
    ResourceRegistry()  = default;
    ~ResourceRegistry() = default;

    void initialise();
    void shutdown();

    void registerResource(const ResourceDefinition& def);
    std::optional<ResourceDefinition> find(const std::string& id) const;
    std::vector<ResourceDefinition> listAll() const;
    std::vector<ResourceDefinition> listByCategory(ResourceCategory cat) const;

private:
    std::vector<ResourceDefinition> resources_;
};

} // namespace NovaForge::Gameplay::Economy
