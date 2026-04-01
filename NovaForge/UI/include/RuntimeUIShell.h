#pragma once

#include <string>
#include <vector>

class RuntimeUIShell
{
public:
    bool Initialize();
    void Tick(float DeltaTime);
    void Shutdown();
    void PushMessage(const std::string& Message);

private:
    std::vector<std::string> Messages;
};
