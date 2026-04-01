#include "Voxel/StructureRegistry.h"
#include <iostream>

bool StructureRegistry::Initialize()
{
    std::cout << "[Structures] Initialize\n";
    RegisterStructure({"ship_dev_001", "Dev Ship", 16, 8, 16});
    return true;
}

bool StructureRegistry::RegisterStructure(const StructureRecord& Record)
{
    auto [It, Inserted] = Structures.emplace(Record.StructureId, Record);
    if (Inserted)
    {
        std::cout << "[Structures] Registered " << Record.StructureId << "\n";
    }
    return Inserted;
}

const StructureRecord* StructureRegistry::FindStructure(const std::string& StructureId) const
{
    auto It = Structures.find(StructureId);
    return It != Structures.end() ? &It->second : nullptr;
}

std::vector<StructureRecord> StructureRegistry::GetAllStructures() const
{
    std::vector<StructureRecord> Out;
    Out.reserve(Structures.size());
    for (const auto& [Key, Value] : Structures)
    {
        Out.push_back(Value);
    }
    return Out;
}

void StructureRegistry::Shutdown()
{
    std::cout << "[Structures] Shutdown\n";
    Structures.clear();
}
