// UnifiedDataRegistry.h
// NovaForge Data — central registry for all canonical game records (items, recipes,
// missions, factions, modules). Provides centralised ID lookup, cross-reference
// validation, schema version checks, and hot-reload hooks.

#pragma once
#include "Data/UnifiedDataTypes.h"

#include <functional>
#include <optional>
#include <string>
#include <vector>

namespace novaforge::data {

// ---------------------------------------------------------------------------
// Validation result
// ---------------------------------------------------------------------------

enum class EValidationSeverity : uint8_t { Info, Warning, Error };

struct ValidationIssue
{
    std::string          recordType;  ///< "Item", "Recipe", "Mission", etc.
    std::string          recordId;
    std::string          field;
    std::string          message;
    EValidationSeverity  severity = EValidationSeverity::Error;
};

struct ValidationReport
{
    std::vector<ValidationIssue> issues;
    bool IsClean() const {
        for (const auto& i : issues)
            if (i.severity == EValidationSeverity::Error) return false;
        return true;
    }
    size_t ErrorCount() const {
        size_t c = 0;
        for (const auto& i : issues)
            if (i.severity == EValidationSeverity::Error) ++c;
        return c;
    }
};

// ---------------------------------------------------------------------------
// Hot-reload callback
// ---------------------------------------------------------------------------

using HotReloadCallback = std::function<void(const std::string& recordType,
                                              const std::string& recordId)>;

// ---------------------------------------------------------------------------
// UnifiedDataRegistry
// ---------------------------------------------------------------------------

class UnifiedDataRegistry
{
public:
    UnifiedDataRegistry()  = default;
    ~UnifiedDataRegistry() = default;

    bool Initialize();
    void Shutdown();

    // ---- registration -----------------------------------------------
    void RegisterItem   (const ItemRecord&    item);
    void RegisterRecipe (const RecipeRecord&  recipe);
    void RegisterMission(const MissionRecord& mission);
    void RegisterFaction(const FactionRecord& faction);
    void RegisterModule (const ModuleRecord&  module);

    // ---- lookup (centralised ID lookup) -----------------------------
    std::optional<ItemRecord>    FindItem   (const std::string& id) const;
    std::optional<RecipeRecord>  FindRecipe (const std::string& id) const;
    std::optional<MissionRecord> FindMission(const std::string& id) const;
    std::optional<FactionRecord> FindFaction(uint32_t factionId)    const;
    std::optional<ModuleRecord>  FindModule (const std::string& id) const;

    // Convenience: check existence without retrieving.
    bool HasItem   (const std::string& id) const;
    bool HasRecipe (const std::string& id) const;
    bool HasMission(const std::string& id) const;
    bool HasFaction(uint32_t factionId)    const;
    bool HasModule (const std::string& id) const;

    // ---- list all of each type -------------------------------------
    std::vector<ItemRecord>    ListItems()    const { return m_items; }
    std::vector<RecipeRecord>  ListRecipes()  const { return m_recipes; }
    std::vector<MissionRecord> ListMissions() const { return m_missions; }
    std::vector<FactionRecord> ListFactions() const { return m_factions; }
    std::vector<ModuleRecord>  ListModules()  const { return m_modules; }

    // ---- cross-reference validation ---------------------------------
    ValidationReport ValidateAll()           const;
    ValidationReport ValidateRecipes()       const;
    ValidationReport ValidateMissions()      const;
    ValidationReport ValidateModules()       const;

    // ---- schema version checks --------------------------------------
    bool CheckSchemaVersion(const std::string& recordType,
                             uint16_t requiredMajor) const;

    // ---- hot reload -------------------------------------------------
    void SetHotReloadCallback(HotReloadCallback cb) { m_reloadCb = std::move(cb); }
    void NotifyHotReload(const std::string& recordType, const std::string& recordId);

    // ---- editor writeback -------------------------------------------
    /// Replace an existing item definition (editor writes back changes).
    bool WritebackItem   (const ItemRecord&    item);
    bool WritebackRecipe (const RecipeRecord&  recipe);
    bool WritebackMission(const MissionRecord& mission);

    // ---- stats -------------------------------------------------------
    size_t TotalRecords() const;

private:
    std::vector<ItemRecord>    m_items;
    std::vector<RecipeRecord>  m_recipes;
    std::vector<MissionRecord> m_missions;
    std::vector<FactionRecord> m_factions;
    std::vector<ModuleRecord>  m_modules;

    HotReloadCallback m_reloadCb;

    void AddValidationIssue(ValidationReport& report,
                             const std::string& type,
                             const std::string& id,
                             const std::string& field,
                             const std::string& msg,
                             EValidationSeverity sev = EValidationSeverity::Error) const;
};

} // namespace novaforge::data
