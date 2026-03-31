#pragma once

#include <memory>
#include <string>

class DataRegistry;
class PlayerController;
class InventorySystem;
class CraftingSystem;
class InteractionSystem;
class MissionSystem;

class GameplayManager
{
public:
    bool Initialize(DataRegistry& InDataRegistry);
    void Tick(float DeltaTime);
    void Shutdown();

    void RunDemoLoop();

    PlayerController& GetPlayerController();
    InventorySystem& GetInventorySystem();
    CraftingSystem& GetCraftingSystem();
    InteractionSystem& GetInteractionSystem();
    MissionSystem& GetMissionSystem();

private:
    DataRegistry* Data = nullptr;
    std::unique_ptr<PlayerController> Player;
    std::unique_ptr<InventorySystem> Inventory;
    std::unique_ptr<CraftingSystem> Crafting;
    std::unique_ptr<InteractionSystem> Interaction;
    std::unique_ptr<MissionSystem> Missions;
};
