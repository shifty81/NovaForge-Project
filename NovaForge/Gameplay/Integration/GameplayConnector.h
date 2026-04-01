// GameplayConnector.h
// NovaForge Integration — connects salvage/mining completion to loot resolution,
// inventory insertion, HUD feedback events, and mission progress hooks.

#pragma once
#include <cstdint>
#include <functional>
#include <string>
#include <vector>

namespace novaforge::integration {

// ---------------------------------------------------------------------------
// HUD feedback event types
// ---------------------------------------------------------------------------

enum class EHUDFeedbackType : uint8_t
{
    ItemReceived,       ///< item inserted into inventory
    CreditsGained,
    MissionProgress,
    ResourceExtracted,
    LootTableRolled,
    NotificationAlert,
};

struct HUDFeedbackEvent
{
    EHUDFeedbackType type       = EHUDFeedbackType::ItemReceived;
    uint64_t         playerId   = 0;
    std::string      label;         ///< human-readable summary
    std::string      itemId;        ///< for ItemReceived
    float            amount        = 0.f;  ///< credits or units
    uint32_t         quantity      = 0;
};

// ---------------------------------------------------------------------------
// Salvage completion event (produced when salvage job finishes)
// ---------------------------------------------------------------------------

struct SalvageCompletionEvent
{
    uint64_t    playerId      = 0;
    uint64_t    wreckEntityId = 0;
    std::string lootTableId;
    uint64_t    randomSeed    = 0;
    float       creditMultiplier = 1.0f;
};

// ---------------------------------------------------------------------------
// Mining completion event (produced when extraction cycle finishes)
// ---------------------------------------------------------------------------

struct MiningCompletionEvent
{
    uint64_t    playerId        = 0;
    uint64_t    nodeId          = 0;
    std::string resourceType;
    float       unitsExtracted  = 0.f;
};

// ---------------------------------------------------------------------------
// Mission progress hook
// ---------------------------------------------------------------------------

struct MissionProgressEvent
{
    uint64_t    playerId    = 0;
    std::string missionId;
    std::string objectiveId;
    float       progressDelta = 1.f;
};

// ---------------------------------------------------------------------------
// Reward summary (for HUD + logging)
// ---------------------------------------------------------------------------

struct RewardSummary
{
    uint64_t                  playerId         = 0;
    float                     creditsAwarded    = 0.f;
    std::vector<std::string>  itemIds;
    std::vector<uint32_t>     quantities;
    int32_t                   itemTypesInserted = 0;
    bool                      missionAdvanced   = false;
};

// ---------------------------------------------------------------------------
// HUD feedback callback
// ---------------------------------------------------------------------------

using HUDFeedbackCallback     = std::function<void(const HUDFeedbackEvent&)>;
using MissionProgressCallback = std::function<void(const MissionProgressEvent&)>;

// ---------------------------------------------------------------------------
// GameplayConnector
// ---------------------------------------------------------------------------

class GameplayConnector
{
public:
    bool Initialize();
    void Shutdown();

    // ---- callbacks -------------------------------------------------------
    void SetHUDCallback          (HUDFeedbackCallback cb)
    { m_hudCb = std::move(cb); }
    void SetMissionProgressCallback(MissionProgressCallback cb)
    { m_missionCb = std::move(cb); }

    // ---- salvage completion pipeline ------------------------------------
    /// Resolve loot table → insert items into inventory → fire HUD events.
    /// Returns summary of what was awarded.
    RewardSummary OnSalvageCompleted(const SalvageCompletionEvent& evt);

    // ---- mining completion pipeline ------------------------------------
    /// Convert extracted resources → inventory insertion → fire HUD events.
    RewardSummary OnMiningCompleted(const MiningCompletionEvent& evt);

    // ---- mission progress -----------------------------------------------
    void AdvanceMissionObjective(const MissionProgressEvent& evt);

    // ---- manual reward pipeline ----------------------------------------
    RewardSummary ApplyLootReward(uint64_t playerId,
                                    const std::string& lootTableId,
                                    float creditMultiplier = 1.0f,
                                    uint64_t seed = 0);

    // ---- credit delivery -----------------------------------------------
    void AwardCredits(uint64_t playerId, float amount, const std::string& reason = "");

    // ---- event log query -----------------------------------------------
    const std::vector<HUDFeedbackEvent>& GetRecentHUDEvents() const
    { return m_hudLog; }
    void ClearHUDLog() { m_hudLog.clear(); }

    size_t TotalRewardsIssued() const { return m_rewardCount; }

private:
    HUDFeedbackCallback       m_hudCb;
    MissionProgressCallback   m_missionCb;
    std::vector<HUDFeedbackEvent> m_hudLog;
    size_t                    m_rewardCount = 0;

    void FireHUD(const HUDFeedbackEvent& evt);
};

} // namespace novaforge::integration
