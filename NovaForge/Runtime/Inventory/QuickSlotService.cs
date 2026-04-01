using System;
using Runtime.NovaForge.Player;

namespace Runtime.NovaForge.Inventory;

public interface IQuickSlotService
{
    string? GetActiveSlot(PlayerRigState state, int index);
    void Activate(PlayerRigState state, int index);
}

public sealed class QuickSlotService : IQuickSlotService
{
    public string? GetActiveSlot(PlayerRigState state, int index)
    {
        if (index < 0 || index >= state.QuickSlots.Count)
        {
            return null;
        }

        return state.QuickSlots[index];
    }

    public void Activate(PlayerRigState state, int index)
    {
        var itemId = GetActiveSlot(state, index);
        if (itemId is null)
        {
            throw new InvalidOperationException($"Quick slot {index} is empty or invalid.");
        }

        // TODO: bridge to real item/tool activation logic.
    }
}
