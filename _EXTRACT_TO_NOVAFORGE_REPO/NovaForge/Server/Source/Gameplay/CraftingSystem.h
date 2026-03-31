#pragma once

#include <string>

class DataRegistry;
class InventorySystem;

class CraftingSystem
{
public:
    bool Initialize(DataRegistry& InDataRegistry, InventorySystem& InInventory);
    bool CraftRecipe(const std::string& RecipeId);

private:
    DataRegistry* Data = nullptr;
    InventorySystem* Inventory = nullptr;
};
