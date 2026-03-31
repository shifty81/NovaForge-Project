#include <iostream>
#include "MiningTypes.h"

void ProcessMining(MiningNode& Node)
{
    if (Node.bDepleted) return;

    Node.ResourceAmount -= 20.0f;

    std::cout << "[Mining] Extracting " << Node.ResourceType 
              << " Remaining=" << Node.ResourceAmount << "\n";

    if (Node.ResourceAmount <= 0.0f)
    {
        Node.bDepleted = true;
        std::cout << "[Mining] Node depleted\n";
    }
}