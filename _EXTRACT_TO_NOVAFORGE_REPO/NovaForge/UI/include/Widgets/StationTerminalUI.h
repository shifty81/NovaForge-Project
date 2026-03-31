// StationTerminalUI.h
// NovaForge UI — station terminal screen: storage, repair/resupply, manufacturing.

#pragma once
#include "Widgets/UIWidgetBase.h"

#include <string>
#include <vector>

namespace novaforge::ui {

enum class EStationTab : uint8_t
{
    Storage,
    Manufacturing,
    Repair,
    Resupply,
    Market,
};

struct StationStorageRow
{
    std::string itemId;
    std::string displayName;
    uint32_t    quantity = 0;
};

struct StationManufacturingRow
{
    std::string jobId;
    std::string outputItemName;
    float       progressPct = 0.f; ///< 0-100
    std::string statusLabel;
};

class StationTerminalUI
{
public:
    bool Initialize();
    void Shutdown();

    // ---- open / close -----------------------------------------------
    void Open(uint64_t stationId);
    void Close();
    bool IsOpen() const { return m_open; }
    uint64_t GetStationId() const { return m_stationId; }

    // ---- tab control ------------------------------------------------
    void SetTab(EStationTab tab) { m_activeTab = tab; }
    EStationTab GetTab()  const  { return m_activeTab; }

    // ---- data binding -----------------------------------------------
    void SetStorageRows(const std::vector<StationStorageRow>& rows);
    void SetManufacturingRows(const std::vector<StationManufacturingRow>& rows);
    void SetRepairCost(float cost)  { m_repairCost  = cost; }
    void SetResupplyCost(float cost){ m_resupplyCost = cost; }

    // ---- interaction callbacks --------------------------------------
    using DepositCallback    = std::function<void(const std::string& itemId, uint32_t qty)>;
    using WithdrawCallback   = std::function<void(const std::string& itemId, uint32_t qty)>;
    using RepairCallback     = std::function<void()>;
    using ResupplyCallback   = std::function<void()>;

    void SetDepositCallback  (DepositCallback  cb) { m_depositCb  = std::move(cb); }
    void SetWithdrawCallback (WithdrawCallback cb) { m_withdrawCb = std::move(cb); }
    void SetRepairCallback   (RepairCallback   cb) { m_repairCb   = std::move(cb); }
    void SetResupplyCallback (ResupplyCallback cb) { m_resupplyCb = std::move(cb); }

    void TriggerRepair()   { if (m_repairCb)   m_repairCb(); }
    void TriggerResupply() { if (m_resupplyCb) m_resupplyCb(); }

    // ---- widget export ----------------------------------------------
    std::vector<UIWidget> BuildWidgets() const;

private:
    uint64_t                           m_stationId    = 0;
    bool                               m_open         = false;
    EStationTab                        m_activeTab    = EStationTab::Storage;
    std::vector<StationStorageRow>     m_storageRows;
    std::vector<StationManufacturingRow> m_mfgRows;
    float                              m_repairCost   = 0.f;
    float                              m_resupplyCost = 0.f;

    DepositCallback    m_depositCb;
    WithdrawCallback   m_withdrawCb;
    RepairCallback     m_repairCb;
    ResupplyCallback   m_resupplyCb;
};

} // namespace novaforge::ui
