#include "Gameplay/CraftingSystem.h"
#include "Data/DataRegistry.h"
#include "Gameplay/InventorySystem.h"
#include <iostream>

bool CraftingSystem::Initialize(DataRegistry& InDataRegistry, InventorySystem& InInventory)
{
    Data = &InDataRegistry;
    Inventory = &InInventory;
    std::cout << "[CraftingSystem] Initialize\n";
    return true;
}

bool CraftingSystem::CraftRecipe(const std::string& RecipeId)
{
    if (!Data || !Inventory)
    {
        return false;
    }

    const auto* Recipe = Data->FindRecipeDefinition(RecipeId);
    if (!Recipe)
    {
        std::cout << "[CraftingSystem] Missing recipe " << RecipeId << "\n";
        return false;
    }

    for (const auto& Input : Recipe->Inputs)
    {
        if (!Inventory->HasItem(Input.ItemId, Input.Count))
        {
            std::cout << "[CraftingSystem] Missing input " << Input.ItemId << "\n";
            return false;
        }
    }

    for (const auto& Input : Recipe->Inputs)
    {
        Inventory->RemoveItem(Input.ItemId, Input.Count);
    }

    for (const auto& Output : Recipe->Outputs)
    {
        Inventory->AddItem(Output.ItemId, Output.Count);
    }

    std::cout << "[CraftingSystem] Crafted " << Recipe->Id << "\n";
    return true;
}
