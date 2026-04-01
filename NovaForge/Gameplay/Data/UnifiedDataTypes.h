// UnifiedDataTypes.h
// NovaForge Data — canonical runtime record types for items, recipes, missions,
// factions, and modules — the single source of truth for all gameplay data.

#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace novaforge::data {

// ---------------------------------------------------------------------------
// Schema versioning
// ---------------------------------------------------------------------------

struct SchemaVersion
{
    uint16_t major = 1;
    uint16_t minor = 0;
    bool isCompatible(uint16_t reqMajor) const { return major == reqMajor; }
};

// ---------------------------------------------------------------------------
// Item record
// ---------------------------------------------------------------------------

struct ItemRecord
{
    std::string  id;
    std::string  displayName;
    std::string  categoryTag;   ///< Resource, Component, Weapon, Module, etc.
    float        massPerUnit   = 0.1f;
    float        volumePerUnit = 0.01f;
    float        baseValue     = 1.0f;
    bool         isStackable   = true;
    uint32_t     maxStackSize  = 9999;
    bool         isLegal       = true;
    SchemaVersion schemaVersion;
};

// ---------------------------------------------------------------------------
// Recipe record (manufacturing)
// ---------------------------------------------------------------------------

struct RecipeIngredient
{
    std::string itemId;
    uint32_t    quantity = 1;
};

struct RecipeRecord
{
    std::string                   id;
    std::string                   outputItemId;
    uint32_t                      outputQuantity   = 1;
    std::vector<RecipeIngredient> ingredients;
    float                         productionTime   = 60.f;  ///< seconds
    std::string                   requiredFacility; ///< e.g. "basic_fab"
    SchemaVersion                 schemaVersion;
};

// ---------------------------------------------------------------------------
// Mission record
// ---------------------------------------------------------------------------

enum class EMissionType : uint8_t
{
    Delivery, Combat, Salvage, Mining, Escort, Exploration, Bounty
};

struct MissionRecord
{
    std::string   id;
    std::string   title;
    std::string   description;
    EMissionType  type            = EMissionType::Delivery;
    uint32_t      issuingFactionId = 0;
    float         creditReward    = 0.f;
    float         reputationReward = 0.f;
    std::string   skillRewardId;
    float         skillXPReward   = 0.f;
    std::string   tierTag;         ///< "tier1", "tier2", …
    SchemaVersion schemaVersion;
};

// ---------------------------------------------------------------------------
// Faction record
// ---------------------------------------------------------------------------

struct FactionRecord
{
    uint32_t    id           = 0;
    std::string name;
    std::string description;
    float       baseHostility = 0.f;  ///< -1 friendly → +1 hostile
    std::vector<uint32_t> alliedWith;
    std::vector<uint32_t> hostileWith;
    SchemaVersion schemaVersion;
};

// ---------------------------------------------------------------------------
// Module record (ship / structure module)
// ---------------------------------------------------------------------------

enum class EModuleSlot : uint8_t
{
    Weapon, Engine, Shield, Reactor, Utility, Structure
};

struct ModuleRecord
{
    std::string   id;
    std::string   displayName;
    EModuleSlot   slot          = EModuleSlot::Utility;
    float         powerDraw     = 0.f;
    float         mass          = 1.0f;
    std::string   requiredItemId; ///< item needed to craft/place
    SchemaVersion schemaVersion;
};

} // namespace novaforge::data
