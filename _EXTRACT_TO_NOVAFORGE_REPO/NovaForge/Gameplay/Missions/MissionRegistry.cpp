// MissionRegistry.cpp
#include "MissionRegistry.h"
#include <algorithm>

namespace NovaForge::Gameplay::Missions
{

void MissionRegistry::initialise() {}
void MissionRegistry::shutdown()   {}

void MissionRegistry::registerTemplate(const MissionTemplate& tmpl)
{
    templates_.push_back(tmpl);
}

std::optional<MissionTemplate> MissionRegistry::findTemplate(uint32_t id) const
{
    for (const auto& t : templates_)
        if (t.templateId == id) return t;
    return std::nullopt;
}

std::vector<MissionTemplate> MissionRegistry::listTemplates(MissionType type) const
{
    std::vector<MissionTemplate> result;
    for (const auto& t : templates_)
        if (t.type == type) result.push_back(t);
    return result;
}

MissionInstance MissionRegistry::createInstance(uint64_t playerId, uint32_t templateId)
{
    MissionInstance inst;
    inst.instanceId = nextInstanceId_++;
    inst.playerId   = playerId;
    inst.templateId = templateId;
    inst.status     = MissionStatus::Active;
    instances_.push_back(inst);
    return inst;
}

bool MissionRegistry::updateStatus(uint64_t instanceId, MissionStatus status)
{
    for (auto& i : instances_)
    {
        if (i.instanceId == instanceId)
        {
            i.status = status;
            return true;
        }
    }
    return false;
}

std::optional<MissionInstance> MissionRegistry::findInstance(uint64_t id) const
{
    for (const auto& i : instances_)
        if (i.instanceId == id) return i;
    return std::nullopt;
}

std::vector<MissionInstance> MissionRegistry::listActive(uint64_t playerId) const
{
    std::vector<MissionInstance> result;
    for (const auto& i : instances_)
        if (i.playerId == playerId && i.status == MissionStatus::Active)
            result.push_back(i);
    return result;
}

} // namespace NovaForge::Gameplay::Missions
