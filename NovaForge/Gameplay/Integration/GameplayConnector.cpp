// GameplayConnector.cpp
// NovaForge Integration — gameplay connector implementation.

#include "Integration/GameplayConnector.h"

namespace novaforge::integration {

bool GameplayConnector::Initialize()
{
    m_rewardCount = 0;
    m_hudLog.clear();
    return true;
}

void GameplayConnector::Shutdown()
{
    m_hudLog.clear();
    m_hudCb       = nullptr;
    m_missionCb   = nullptr;
    m_rewardCount = 0;
}

void GameplayConnector::FireHUD(const HUDFeedbackEvent& evt)
{
    m_hudLog.push_back(evt);
    if (m_hudCb) m_hudCb(evt);
}

RewardSummary GameplayConnector::OnSalvageCompleted(
    const SalvageCompletionEvent& evt)
{
    RewardSummary summary;
    summary.playerId = evt.playerId;

    // Simulate loot table resolution:
    // In a full engine, LootResolver would be invoked here.
    // Produce a placeholder reward representing the pipeline hookup.
    summary.creditsAwarded    = 500.f * evt.creditMultiplier;
    summary.itemIds           = { "scrap_metal", "salvaged_electronics" };
    summary.quantities        = { 5, 2 };
    summary.itemTypesInserted = static_cast<int32_t>(summary.itemIds.size());
    ++m_rewardCount;

    // Fire HUD events for each item.
    for (size_t i = 0; i < summary.itemIds.size(); ++i)
    {
        HUDFeedbackEvent e;
        e.type     = EHUDFeedbackType::ItemReceived;
        e.playerId = evt.playerId;
        e.itemId   = summary.itemIds[i];
        e.quantity = summary.quantities[i];
        e.label    = "Salvage: +" + std::to_string(summary.quantities[i])
                     + "x " + summary.itemIds[i];
        FireHUD(e);
    }

    // Fire credits HUD event.
    if (summary.creditsAwarded > 0.f)
    {
        HUDFeedbackEvent credEvt;
        credEvt.type     = EHUDFeedbackType::CreditsGained;
        credEvt.playerId = evt.playerId;
        credEvt.amount   = summary.creditsAwarded;
        credEvt.label    = "Credits: +" + std::to_string(
                             static_cast<int>(summary.creditsAwarded));
        FireHUD(credEvt);
    }

    // Fire loot table event.
    {
        HUDFeedbackEvent ltEvt;
        ltEvt.type     = EHUDFeedbackType::LootTableRolled;
        ltEvt.playerId = evt.playerId;
        ltEvt.label    = "Loot table rolled: " + evt.lootTableId;
        FireHUD(ltEvt);
    }

    return summary;
}

RewardSummary GameplayConnector::OnMiningCompleted(
    const MiningCompletionEvent& evt)
{
    RewardSummary summary;
    summary.playerId = evt.playerId;
    summary.itemIds           = { evt.resourceType };
    summary.quantities        = { static_cast<uint32_t>(evt.unitsExtracted) };
    summary.itemTypesInserted = 1;
    ++m_rewardCount;

    HUDFeedbackEvent e;
    e.type     = EHUDFeedbackType::ResourceExtracted;
    e.playerId = evt.playerId;
    e.itemId   = evt.resourceType;
    e.amount   = evt.unitsExtracted;
    e.label    = "Mined: +" + std::to_string(
                  static_cast<int>(evt.unitsExtracted))
                 + " " + evt.resourceType;
    FireHUD(e);

    return summary;
}

void GameplayConnector::AdvanceMissionObjective(
    const MissionProgressEvent& evt)
{
    if (m_missionCb) m_missionCb(evt);

    HUDFeedbackEvent e;
    e.type     = EHUDFeedbackType::MissionProgress;
    e.playerId = evt.playerId;
    e.label    = "Mission progress: " + evt.missionId
                 + " / " + evt.objectiveId;
    FireHUD(e);
}

RewardSummary GameplayConnector::ApplyLootReward(
    uint64_t playerId,
    const std::string& lootTableId,
    float creditMultiplier,
    uint64_t seed)
{
    SalvageCompletionEvent evt;
    evt.playerId         = playerId;
    evt.lootTableId      = lootTableId;
    evt.randomSeed       = seed;
    evt.creditMultiplier = creditMultiplier;
    return OnSalvageCompleted(evt);
}

void GameplayConnector::AwardCredits(uint64_t playerId, float amount,
                                       const std::string& reason)
{
    HUDFeedbackEvent e;
    e.type     = EHUDFeedbackType::CreditsGained;
    e.playerId = playerId;
    e.amount   = amount;
    e.label    = reason.empty()
                 ? ("Credits: +" + std::to_string(static_cast<int>(amount)))
                 : reason + " +" + std::to_string(static_cast<int>(amount));
    FireHUD(e);
    ++m_rewardCount;
}

} // namespace novaforge::integration
