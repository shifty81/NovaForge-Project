using Runtime.NovaForge.Inventory;

namespace Runtime.NovaForge.DevWorld.Terminals
{
    public sealed class InventoryCraftingTerminal : IDevTerminal
    {
        private readonly IInventoryDebugService _inventoryDebugService;

        public InventoryCraftingTerminal(IInventoryDebugService inventoryDebugService)
        {
            _inventoryDebugService = inventoryDebugService;
        }

        public string TerminalId => "terminal_inventory_crafting";
        public string DisplayName => "Inventory + Crafting Terminal";

        public void Interact()
        {
            _inventoryDebugService.GrantStarterMaterials();
        }
    }
}
