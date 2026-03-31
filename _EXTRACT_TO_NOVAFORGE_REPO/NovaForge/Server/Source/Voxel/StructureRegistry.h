#pragma once

#include <string>
#include <unordered_map>
#include <vector>

struct StructureRecord
{
    std::string StructureId;
    std::string DisplayName;
    int Width = 0;
    int Height = 0;
    int Depth = 0;
};

class StructureRegistry
{
public:
    bool Initialize();
    bool RegisterStructure(const StructureRecord& Record);
    const StructureRecord* FindStructure(const std::string& StructureId) const;
    std::vector<StructureRecord> GetAllStructures() const;
    void Shutdown();

private:
    std::unordered_map<std::string, StructureRecord> Structures;
};
