#include "RuntimeUIHookup.h"

namespace Runtime::UI
{
RuntimeUIHookup::RuntimeUIHookup()
    : m_hudVisible(false)
    , m_initialized(false)
{
}

RuntimeUIHookup::~RuntimeUIHookup() = default;

void RuntimeUIHookup::Initialize()
{
    // TODO:
    // - acquire RuntimeUIShell reference
    // - register HUD display callbacks with gameplay event bus
    m_initialized = true;
}

void RuntimeUIHookup::ShowHUD()
{
    // TODO:
    // - call RuntimeUIShell::Show
    // - restore last HUD state / layout
    m_hudVisible = true;
}

void RuntimeUIHookup::HideHUD()
{
    // TODO:
    // - call RuntimeUIShell::Hide
    // - suppress HUD update dispatches while hidden
    m_hudVisible = false;
}

void RuntimeUIHookup::UpdateHUD(const std::string& key, const std::string& value)
{
    // TODO:
    // - route key/value pair to appropriate RuntimeUIShell widget
    // - throttle high-frequency updates if needed
    (void)key;
    (void)value;
}

bool RuntimeUIHookup::IsHUDVisible() const
{
    return m_hudVisible;
}
}
