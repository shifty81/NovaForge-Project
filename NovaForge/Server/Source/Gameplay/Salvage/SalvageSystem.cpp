#include <iostream>
#include "SalvageTypes.h"

void ProcessSalvage(SalvageNode& Node)
{
    if (Node.bDepleted) return;

    Node.Integrity -= 25.0f;

    std::cout << "[Salvage] Processing " << Node.Id << " Integrity=" << Node.Integrity << "\n";

    if (Node.Integrity <= 0.0f)
    {
        Node.bDepleted = true;
        std::cout << "[Salvage] Node depleted -> reward from " << Node.LootTableId << "\n";
    }
}