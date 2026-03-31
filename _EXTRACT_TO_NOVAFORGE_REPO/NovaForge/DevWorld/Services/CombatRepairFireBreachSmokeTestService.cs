using Runtime.NovaForge.Combat;
using Runtime.NovaForge.Hazards.Breach;
using Runtime.NovaForge.Hazards.Fire;
using Runtime.NovaForge.Repair;

namespace Runtime.NovaForge.DevWorld.Services;

public sealed class CombatRepairFireBreachSmokeTestService
{
    private readonly ICombatStateService _combat;
    private readonly IBreachService _breach;
    private readonly IFireService _fire;
    private readonly IRepairActionService _repair;

    public CombatRepairFireBreachSmokeTestService(
        ICombatStateService combat,
        IBreachService breach,
        IFireService fire,
        IRepairActionService repair)
    {
        _combat = combat;
        _breach = breach;
        _fire = fire;
        _repair = repair;
    }

    public void Run()
    {
        var profile = new DamageProfile("kinetic_slug_mk1", "kinetic", 45f, 20f, 1.1f, 0.8f);
        var evt = _combat.ApplyDamage("player_rig", "dev_damage_hull_segment", "hull_plate_a", profile);

        if (evt.CausedBreach)
        {
            _breach.CreateBreach(evt.TargetId, evt.ZoneId, 0.5f);
        }

        _fire.Ignite(evt.TargetId, evt.ZoneId, 0.4f, 35f);

        _repair.ApplyBreachPatch(evt.TargetId, evt.ZoneId, new RepairActionDef("emergency_breach_patch", "sealant_gun", 4f));
        _repair.ApplyFireSuppression(evt.TargetId, evt.ZoneId, new RepairActionDef("portable_fire_suppress", "suppression_canister", 3f));
        _repair.ApplyHullRestore(evt.TargetId, evt.ZoneId, new RepairActionDef("field_hull_restore", "welder", 6f));
    }
}
