#pragma once

#include <string>

namespace Runtime::Rendering
{
class FPSRenderingBridge
{
public:
    FPSRenderingBridge();
    ~FPSRenderingBridge();

    bool Initialize();
    void Shutdown();

    void RegisterCharacter(const std::string& characterId);
    void SubmitForRendering(const std::string& characterId);
    bool IsVisible(const std::string& characterId) const;
    void Tick(float dt);

private:
    bool m_initialized;
};
}
