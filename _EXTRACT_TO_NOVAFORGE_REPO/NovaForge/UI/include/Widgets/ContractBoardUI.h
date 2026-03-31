// ContractBoardUI.h
// NovaForge UI — contract board screen: available jobs, requirements, accept button.

#pragma once
#include "Widgets/UIWidgetBase.h"

#include <optional>
#include <string>
#include <vector>

namespace novaforge::ui {

struct ContractBoardEntry
{
    std::string contractId;
    std::string title;
    std::string description;
    float       creditReward     = 0.f;
    float       reputationReward = 0.f;
    std::string tierTag;
    bool        isAvailable      = true;  ///< false = locked by gating
    bool        isSelected       = false;
};

class ContractBoardUI
{
public:
    bool Initialize();
    void Shutdown();

    // ---- data binding -----------------------------------------------
    void SetContracts(const std::vector<ContractBoardEntry>& contracts);
    void MarkContractLocked(const std::string& contractId, bool locked);

    // ---- interaction ------------------------------------------------
    void Open();
    void Close();
    bool IsOpen() const { return m_open; }

    void SelectContract(const std::string& contractId);
    std::optional<ContractBoardEntry> GetSelected() const;
    void AcceptSelected();   ///< fires accepted callback
    void DeclineSelected();

    // ---- callbacks --------------------------------------------------
    using AcceptCallback = std::function<void(const std::string& contractId)>;
    void SetAcceptCallback(AcceptCallback cb) { m_acceptCb = std::move(cb); }

    // ---- widget export ----------------------------------------------
    std::vector<UIWidget> BuildWidgets() const;

    const std::vector<ContractBoardEntry>& GetContracts() const { return m_contracts; }

private:
    std::vector<ContractBoardEntry> m_contracts;
    std::string                     m_selectedId;
    bool                            m_open = false;
    AcceptCallback                  m_acceptCb;

    ContractBoardEntry* GetMutable(const std::string& id);
};

} // namespace novaforge::ui
