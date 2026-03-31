// PCGWorldGen.h
// NovaForge procedural world generation — sectors, star systems, planets.

#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace NovaForge::Gameplay::PCG
{

struct PCGSector
{
    uint64_t    sectorId    = 0;
    std::string name;
    uint32_t    seed        = 0;
    float       sizeAU     = 0.0f;    ///< diameter in AU
    uint32_t    systemCount = 0;
};

struct PCGStarSystem
{
    uint64_t    systemId    = 0;
    uint64_t    sectorId    = 0;
    std::string name;
    uint32_t    seed        = 0;
    uint32_t    planetCount = 0;
    bool        hasStation  = false;
};

struct PCGGenerationParams
{
    uint32_t    worldSeed     = 42;
    uint32_t    sectorCount   = 100;
    float       avgSectorSize = 10.0f; ///< AU
    float       stationDensity = 0.2f; ///< fraction of systems with stations
};

class PCGWorldGen
{
public:
    PCGWorldGen()  = default;
    ~PCGWorldGen() = default;

    void initialise();
    void shutdown();

    void generate(const PCGGenerationParams& params);
    std::vector<PCGSector>     getSectors()                          const;
    std::vector<PCGStarSystem> getSystemsInSector(uint64_t sectorId) const;
    PCGStarSystem              getSystem(uint64_t systemId)          const;
    void                       clear();

private:
    std::vector<PCGSector>     sectors_;
    std::vector<PCGStarSystem> systems_;
};

} // namespace NovaForge::Gameplay::PCG
