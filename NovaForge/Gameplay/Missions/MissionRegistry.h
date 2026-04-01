// MissionRegistry.h
// NovaForge mission registry — contract storage, generation, and instance tracking.

#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <optional>

namespace NovaForge::Gameplay::Missions
{

enum class MissionType : uint8_t
{
    Courier, Bounty, Mining, Exploration, Escort, Combat, Construction
};

enum class MissionStatus : uint8_t
{
    Available, Active, Completed, Failed, Expired
};

struct MissionTemplate
{
    uint32_t        templateId  = 0;
    std::string     title;
    std::string     description;
    MissionType     type        = MissionType::Courier;
    uint32_t        factionId   = 0;
    float           baseReward  = 0.0f;
    float           standingReward = 0.0f;
};

struct MissionInstance
{
    uint64_t        instanceId  = 0;
    uint64_t        playerId    = 0;
    uint32_t        templateId  = 0;
    MissionStatus   status      = MissionStatus::Available;
    std::string     targetInfo;
};

class MissionRegistry
{
public:
    MissionRegistry()  = default;
    ~MissionRegistry() = default;

    void initialise();
    void shutdown();

    void registerTemplate(const MissionTemplate& tmpl);
    std::optional<MissionTemplate> findTemplate(uint32_t templateId) const;
    std::vector<MissionTemplate>   listTemplates(MissionType type) const;

    MissionInstance createInstance(uint64_t playerId, uint32_t templateId);
    bool updateStatus(uint64_t instanceId, MissionStatus status);
    std::optional<MissionInstance> findInstance(uint64_t instanceId) const;
    std::vector<MissionInstance>   listActive(uint64_t playerId) const;

private:
    std::vector<MissionTemplate> templates_;
    std::vector<MissionInstance> instances_;
    uint64_t nextInstanceId_ = 1;
};

} // namespace NovaForge::Gameplay::Missions
