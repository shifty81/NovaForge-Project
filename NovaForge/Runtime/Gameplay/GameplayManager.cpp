#include "Gameplay/GameplayManager.h"
#include "Data/DataRegistry.h"
#include "Gameplay/CraftingSystem.h"
#include "Gameplay/InteractionSystem.h"
#include "Gameplay/InventorySystem.h"
#include "Gameplay/MissionSystem.h"
#include "Gameplay/PlayerController.h"
#include <iostream>

bool GameplayManager::Initialize(DataRegistry& InDataRegistry)
{
    Data = &InDataRegistry;

    Player = std::make_unique<PlayerController>();
    Inventory = std::make_unique<InventorySystem>();
    Crafting = std::make_unique<CraftingSystem>();
    Interaction = std::make_unique<InteractionSystem>();
    Missions = std::make_unique<MissionSystem>();

    if (!Player->Initialize("player_001", "Shifty"))
    {
        return false;
    }

    if (!Inventory->Initialize(InDataRegistry))
    {
        return false;
    }

    if (!Crafting->Initialize(InDataRegistry, *Inventory))
    {
        return false;
    }

    if (!Interaction->Initialize())
    {
        return false;
    }

    if (!Missions->Initialize(InDataRegistry))
    {
        return false;
    }

    Inventory->AddItem("steel_plate", 25);
    Inventory->AddItem("copper_wire", 20);
    Inventory->AddItem("micro_core", 3);

    Missions->AcceptMission("mission_salvage_derelict_001");

    std::cout << "[GameplayManager] Initialized\n";
    return true;
}

void GameplayManager::Tick(float DeltaTime)
{
    if (Player)
    {
        Player->Tick(DeltaTime);
    }
}

void GameplayManager::Shutdown()
{
    std::cout << "[GameplayManager] Shutdown\n";
    Missions.reset();
    Interaction.reset();
    Crafting.reset();
    Inventory.reset();
    Player.reset();
    Data = nullptr;
}

void GameplayManager::RunDemoLoop()
{
    std::cout << "[GameplayManager] Demo Loop Start\n";

    Player->Move(1.0f, 0.0f, 0.0f);
    Player->Look(10.0f, -3.0f);

    Inventory->LogInventory();

    Crafting->CraftRecipe("craft_reactor_mk1");
    Inventory->LogInventory();

    Interaction->TryInteract("module", "mod_0001");

    Missions->LogMissionState();
    Missions->CompleteMission("mission_salvage_derelict_001");
    Missions->LogMissionState();

    std::cout << "[GameplayManager] Demo Loop End\n";
}
