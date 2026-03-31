#pragma once

#include <string>

namespace Runtime::UI
{
class RuntimeUIHookup
{
public:
    RuntimeUIHookup();
    ~RuntimeUIHookup();

    void Initialize();

    void ShowHUD();
    void HideHUD();
    void UpdateHUD(const std::string& key, const std::string& value);

    bool IsHUDVisible() const;

private:
    bool m_hudVisible;
    bool m_initialized;
};
}
