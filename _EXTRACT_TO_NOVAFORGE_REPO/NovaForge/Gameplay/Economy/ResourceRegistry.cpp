// ResourceRegistry.cpp
#include "ResourceRegistry.h"
#include <algorithm>

namespace NovaForge::Gameplay::Economy
{

void ResourceRegistry::initialise() {}
void ResourceRegistry::shutdown()   {}

void ResourceRegistry::registerResource(const ResourceDefinition& def)
{
    resources_.push_back(def);
}

std::optional<ResourceDefinition> ResourceRegistry::find(const std::string& id) const
{
    for (const auto& r : resources_)
        if (r.id == id) return r;
    return std::nullopt;
}

std::vector<ResourceDefinition> ResourceRegistry::listAll() const
{
    return resources_;
}

std::vector<ResourceDefinition> ResourceRegistry::listByCategory(ResourceCategory cat) const
{
    std::vector<ResourceDefinition> result;
    for (const auto& r : resources_)
        if (r.category == cat) result.push_back(r);
    return result;
}

} // namespace NovaForge::Gameplay::Economy
