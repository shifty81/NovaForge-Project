#pragma once

#include <string>
#include <unordered_map>
#include <vector>

class StructureRegistry;

struct ChunkCoord
{
    int X = 0;
    int Y = 0;
    int Z = 0;

    std::string ToKey() const;
};

struct VoxelChunk
{
    ChunkCoord Coord;
    std::vector<int> Cells;
};

class VoxelSubsystem
{
public:
    bool Initialize(StructureRegistry& InStructures);
    void Tick(float DeltaTime);
    void Shutdown();

    bool CreateTestChunk(const std::string& StructureId, const ChunkCoord& Coord, int FillValue);
    std::size_t GetChunkCount() const;

private:
    StructureRegistry* Structures = nullptr;
    std::unordered_map<std::string, VoxelChunk> Chunks;
};
