#include "DataRegistry.h"
#include <iostream>

bool DataRegistry::Initialize(const std::string& DataRoot)
{
    std::cout << "[DataRegistry] Initialize Root=" << DataRoot << "\n";
    return true;
}

void DataRegistry::Shutdown()
{
    std::cout << "[DataRegistry] Shutdown\n";
}
