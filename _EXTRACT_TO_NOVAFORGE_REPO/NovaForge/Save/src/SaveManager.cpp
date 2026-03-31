#include "SaveManager.h"
#include <iostream>

bool SaveManager::Initialize()
{
    std::cout << "[SaveManager] Initialize\n";
    return true;
}

void SaveManager::Shutdown()
{
    std::cout << "[SaveManager] Shutdown\n";
}
