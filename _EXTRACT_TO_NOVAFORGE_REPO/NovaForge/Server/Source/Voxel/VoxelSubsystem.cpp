#include "Voxel/VoxelSubsystem.h"
#include "Voxel/StructureRegistry.h"
#include <iostream>
#include <sstream>

std::string ChunkCoord::ToKey() const
{
    std::ostringstream Out;
    Out << X << "_" << Y << "_" << Z;
    return Out.str();
}

bool VoxelSubsystem::Initialize(StructureRegistry& InStructures)
{
    Structures = &InStructures;
    std::cout << "[Voxels] Initialize\n";
    CreateTestChunk("ship_dev_001", {0, 0, 0}, 1);
    return true;
}

void VoxelSubsystem::Tick(float)
{
    std::cout << "[Voxels] ActiveChunks=" << Chunks.size() << "\n";
}

void VoxelSubsystem::Shutdown()
{
    std::cout << "[Voxels] Shutdown\n";
    Chunks.clear();
    Structures = nullptr;
}

bool VoxelSubsystem::CreateTestChunk(const std::string& StructureId, const ChunkCoord& Coord, int FillValue)
{
    if (!Structures || !Structures->FindStructure(StructureId))
    {
        return false;
    }

    const std::string Key = StructureId + ":" + Coord.ToKey();
    VoxelChunk Chunk;
    Chunk.Coord = Coord;
    Chunk.Cells.assign(16 * 16 * 16, FillValue);

    Chunks[Key] = std::move(Chunk);
    std::cout << "[Voxels] Created Chunk " << Key << " Fill=" << FillValue << "\n";
    return true;
}

std::size_t VoxelSubsystem::GetChunkCount() const
{
    return Chunks.size();
}
