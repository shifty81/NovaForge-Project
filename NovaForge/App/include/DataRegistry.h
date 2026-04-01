#pragma once

#include <string>

class DataRegistry
{
public:
    bool Initialize(const std::string& DataRoot);
    void Shutdown();
};
