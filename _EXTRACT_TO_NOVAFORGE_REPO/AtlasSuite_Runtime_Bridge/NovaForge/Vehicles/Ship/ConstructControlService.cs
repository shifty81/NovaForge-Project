using Runtime.NovaForge.Constructs;

namespace Runtime.NovaForge.Vehicles.Ship;

public sealed class ConstructControlService
{
    private readonly IConstructService _constructService;

    public ConstructControlService(IConstructService constructService)
    {
        _constructService = constructService;
    }

    public ConstructControlSnapshot? GetSnapshot(string constructId)
    {
        var record = _constructService.FindById(constructId);
        if (record == null)
        {
            return null;
        }

        return new ConstructControlSnapshot
        {
            ConstructId = record.ConstructId,
            ConstructClass = record.ConstructClass,
            CargoContainerRef = record.CargoContainerRef,
            ModuleRefs = record.ModuleRefs
        };
    }
}
