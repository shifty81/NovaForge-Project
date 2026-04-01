namespace Runtime.NovaForge.Vehicles;

public interface IPossessionService
{
    PossessionState GetCurrentState(string playerId);
    bool TryPossessMech(string playerId, string mechId);
    bool TryPossessShip(string playerId, string shipId, string entryPointId);
    bool TryReturnToRig(string playerId);
}
