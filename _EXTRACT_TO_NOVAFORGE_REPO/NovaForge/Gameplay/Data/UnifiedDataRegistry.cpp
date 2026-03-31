// UnifiedDataRegistry.cpp
// NovaForge Data — central registry for all canonical game records.

#include "Data/UnifiedDataRegistry.h"

#include <algorithm>

namespace novaforge::data {

bool UnifiedDataRegistry::Initialize() { return true; }
void UnifiedDataRegistry::Shutdown()
{
    m_items.clear();
    m_recipes.clear();
    m_missions.clear();
    m_factions.clear();
    m_modules.clear();
}

// ---- registration -------------------------------------------------------

void UnifiedDataRegistry::RegisterItem(const ItemRecord& item)
{
    for (auto& i : m_items) if (i.id == item.id) { i = item; return; }
    m_items.push_back(item);
}

void UnifiedDataRegistry::RegisterRecipe(const RecipeRecord& recipe)
{
    for (auto& r : m_recipes) if (r.id == recipe.id) { r = recipe; return; }
    m_recipes.push_back(recipe);
}

void UnifiedDataRegistry::RegisterMission(const MissionRecord& mission)
{
    for (auto& m : m_missions) if (m.id == mission.id) { m = mission; return; }
    m_missions.push_back(mission);
}

void UnifiedDataRegistry::RegisterFaction(const FactionRecord& faction)
{
    for (auto& f : m_factions) if (f.id == faction.id) { f = faction; return; }
    m_factions.push_back(faction);
}

void UnifiedDataRegistry::RegisterModule(const ModuleRecord& module)
{
    for (auto& m : m_modules) if (m.id == module.id) { m = module; return; }
    m_modules.push_back(module);
}

// ---- lookup -------------------------------------------------------------

std::optional<ItemRecord> UnifiedDataRegistry::FindItem(const std::string& id) const
{
    for (const auto& i : m_items) if (i.id == id) return i;
    return std::nullopt;
}

std::optional<RecipeRecord> UnifiedDataRegistry::FindRecipe(const std::string& id) const
{
    for (const auto& r : m_recipes) if (r.id == id) return r;
    return std::nullopt;
}

std::optional<MissionRecord> UnifiedDataRegistry::FindMission(const std::string& id) const
{
    for (const auto& m : m_missions) if (m.id == id) return m;
    return std::nullopt;
}

std::optional<FactionRecord> UnifiedDataRegistry::FindFaction(uint32_t factionId) const
{
    for (const auto& f : m_factions) if (f.id == factionId) return f;
    return std::nullopt;
}

std::optional<ModuleRecord> UnifiedDataRegistry::FindModule(const std::string& id) const
{
    for (const auto& m : m_modules) if (m.id == id) return m;
    return std::nullopt;
}

bool UnifiedDataRegistry::HasItem   (const std::string& id) const { return FindItem(id).has_value(); }
bool UnifiedDataRegistry::HasRecipe (const std::string& id) const { return FindRecipe(id).has_value(); }
bool UnifiedDataRegistry::HasMission(const std::string& id) const { return FindMission(id).has_value(); }
bool UnifiedDataRegistry::HasFaction(uint32_t fid)          const { return FindFaction(fid).has_value(); }
bool UnifiedDataRegistry::HasModule (const std::string& id) const { return FindModule(id).has_value(); }

// ---- cross-reference validation -----------------------------------------

void UnifiedDataRegistry::AddValidationIssue(
    ValidationReport& report,
    const std::string& type,
    const std::string& id,
    const std::string& field,
    const std::string& msg,
    EValidationSeverity sev) const
{
    report.issues.push_back({ type, id, field, msg, sev });
}

ValidationReport UnifiedDataRegistry::ValidateRecipes() const
{
    ValidationReport report;
    for (const auto& recipe : m_recipes)
    {
        // Output item must exist.
        if (!HasItem(recipe.outputItemId))
            AddValidationIssue(report, "Recipe", recipe.id, "outputItemId",
                               "References missing item: " + recipe.outputItemId);

        // All ingredients must reference known items.
        for (const auto& ing : recipe.ingredients)
        {
            if (!HasItem(ing.itemId))
                AddValidationIssue(report, "Recipe", recipe.id, "ingredient",
                                   "Ingredient references missing item: " + ing.itemId);
        }

        // Sanity: production time must be > 0.
        if (recipe.productionTime <= 0.f)
            AddValidationIssue(report, "Recipe", recipe.id, "productionTime",
                               "productionTime must be > 0",
                               EValidationSeverity::Warning);
    }
    return report;
}

ValidationReport UnifiedDataRegistry::ValidateMissions() const
{
    ValidationReport report;
    for (const auto& mission : m_missions)
    {
        // Issuing faction must exist (0 = unclaimed, allowed).
        if (mission.issuingFactionId != 0 && !HasFaction(mission.issuingFactionId))
            AddValidationIssue(report, "Mission", mission.id, "issuingFactionId",
                               "References missing faction: " +
                               std::to_string(mission.issuingFactionId));

        // Skill reward item must exist if specified.
        if (!mission.skillRewardId.empty() && !HasItem(mission.skillRewardId))
            AddValidationIssue(report, "Mission", mission.id, "skillRewardId",
                               "References missing item: " + mission.skillRewardId,
                               EValidationSeverity::Warning);
    }
    return report;
}

ValidationReport UnifiedDataRegistry::ValidateModules() const
{
    ValidationReport report;
    for (const auto& mod : m_modules)
    {
        if (!mod.requiredItemId.empty() && !HasItem(mod.requiredItemId))
            AddValidationIssue(report, "Module", mod.id, "requiredItemId",
                               "References missing item: " + mod.requiredItemId);
    }
    return report;
}

ValidationReport UnifiedDataRegistry::ValidateAll() const
{
    ValidationReport combined;

    auto merge = [&](const ValidationReport& r)
    {
        for (const auto& issue : r.issues)
            combined.issues.push_back(issue);
    };

    merge(ValidateRecipes());
    merge(ValidateMissions());
    merge(ValidateModules());

    return combined;
}

// ---- schema version checks ----------------------------------------------

bool UnifiedDataRegistry::CheckSchemaVersion(const std::string& recordType,
                                               uint16_t requiredMajor) const
{
    if (recordType == "Item")
    {
        for (const auto& i : m_items)
            if (!i.schemaVersion.isCompatible(requiredMajor)) return false;
        return true;
    }
    if (recordType == "Recipe")
    {
        for (const auto& r : m_recipes)
            if (!r.schemaVersion.isCompatible(requiredMajor)) return false;
        return true;
    }
    if (recordType == "Mission")
    {
        for (const auto& m : m_missions)
            if (!m.schemaVersion.isCompatible(requiredMajor)) return false;
        return true;
    }
    return true; // unknown type — pass
}

// ---- hot reload ---------------------------------------------------------

void UnifiedDataRegistry::NotifyHotReload(const std::string& recordType,
                                            const std::string& recordId)
{
    if (m_reloadCb) m_reloadCb(recordType, recordId);
}

// ---- editor writeback ---------------------------------------------------

bool UnifiedDataRegistry::WritebackItem(const ItemRecord& item)
{
    for (auto& i : m_items)
        if (i.id == item.id) { i = item; NotifyHotReload("Item", item.id); return true; }
    return false;
}

bool UnifiedDataRegistry::WritebackRecipe(const RecipeRecord& recipe)
{
    for (auto& r : m_recipes)
        if (r.id == recipe.id) { r = recipe; NotifyHotReload("Recipe", recipe.id); return true; }
    return false;
}

bool UnifiedDataRegistry::WritebackMission(const MissionRecord& mission)
{
    for (auto& m : m_missions)
        if (m.id == mission.id) { m = mission; NotifyHotReload("Mission", mission.id); return true; }
    return false;
}

// ---- stats --------------------------------------------------------------

size_t UnifiedDataRegistry::TotalRecords() const
{
    return m_items.size() + m_recipes.size() + m_missions.size() +
           m_factions.size() + m_modules.size();
}

} // namespace novaforge::data
