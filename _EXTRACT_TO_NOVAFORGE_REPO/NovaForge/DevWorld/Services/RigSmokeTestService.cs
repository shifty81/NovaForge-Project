using System.Threading;
using System.Threading.Tasks;
using Runtime.NovaForge.Inventory;
using Runtime.NovaForge.Player;

namespace Runtime.NovaForge.DevWorld.Services;

public interface IRigSmokeTestService
{
    Task<bool> RunAsync(string playerId, CancellationToken cancellationToken = default);
}

public sealed class RigSmokeTestService : IRigSmokeTestService
{
    private readonly IRigBootstrapService _rigBootstrapService;
    private readonly IQuickSlotService _quickSlotService;

    public RigSmokeTestService(IRigBootstrapService rigBootstrapService, IQuickSlotService quickSlotService)
    {
        _rigBootstrapService = rigBootstrapService;
        _quickSlotService = quickSlotService;
    }

    public async Task<bool> RunAsync(string playerId, CancellationToken cancellationToken = default)
    {
        var state = await _rigBootstrapService.SpawnDefaultRigAsync(playerId, cancellationToken);
        _quickSlotService.Activate(state, 0);
        return state.PlayerId == playerId && state.QuickSlots.Count > 0;
    }
}
