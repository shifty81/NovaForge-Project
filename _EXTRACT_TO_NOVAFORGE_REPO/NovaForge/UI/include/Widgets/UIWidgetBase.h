// UIWidgetBase.h
// NovaForge UI — base widget interface and common widget types.

#pragma once
#include <cstdint>
#include <functional>
#include <string>
#include <vector>

namespace novaforge::ui {

enum class EWidgetType : uint8_t
{
    Panel,
    Label,
    Button,
    ProgressBar,
    ListView,
    Slot,          ///< inventory / hotbar slot
    Icon,
    Separator,
};

enum class EWidgetState : uint8_t
{
    Normal,
    Hovered,
    Pressed,
    Disabled,
    Hidden,
};

struct WidgetRect
{
    float x = 0.f, y = 0.f;    ///< top-left (normalised 0-1 or pixel depending on context)
    float w = 0.f, h = 0.f;
};

using WidgetClickCallback = std::function<void(const std::string& widgetId)>;

struct UIWidget
{
    std::string       widgetId;
    EWidgetType       type       = EWidgetType::Panel;
    EWidgetState      state      = EWidgetState::Normal;
    std::string       label;
    WidgetRect        rect;
    bool              visible    = true;
    bool              enabled    = true;
    float             progress   = 0.f;   ///< for ProgressBar widgets (0-1)
    std::string       iconId;             ///< for Icon/Slot widgets
    WidgetClickCallback onClick;
};

} // namespace novaforge::ui
