// PCGWorldGen.cpp
#include "PCGWorldGen.h"
#include <random>
#include <sstream>

namespace NovaForge::Gameplay::PCG
{

void PCGWorldGen::initialise() {}
void PCGWorldGen::shutdown()   {}

void PCGWorldGen::generate(const PCGGenerationParams& p)
{
    clear();
    std::mt19937 rng(p.worldSeed);
    std::uniform_int_distribution<uint32_t> seedDist;
    std::uniform_int_distribution<uint32_t> sysDist(3, 10);
    std::uniform_int_distribution<uint32_t> planetDist(1, 8);
    std::uniform_int_distribution<uint32_t> pct(0, 99);
    std::uniform_real_distribution<float>   sizeMult(0.5f, 1.5f);

    for (uint32_t i = 0; i < p.sectorCount; ++i)
    {
        PCGSector sec;
        sec.sectorId    = static_cast<uint64_t>(i + 1);
        sec.seed        = seedDist(rng);
        sec.sizeAU      = p.avgSectorSize * sizeMult(rng);
        sec.systemCount = sysDist(rng);
        std::ostringstream n; n << "Sector-" << (i + 1);
        sec.name = n.str();
        sectors_.push_back(sec);

        for (uint32_t j = 0; j < sec.systemCount; ++j)
        {
            PCGStarSystem sys;
            sys.systemId    = static_cast<uint64_t>(i * 100 + j + 1);
            sys.sectorId    = sec.sectorId;
            sys.seed        = seedDist(rng);
            sys.planetCount = planetDist(rng);
            sys.hasStation  = (pct(rng) < static_cast<uint32_t>(p.stationDensity * 100));
            std::ostringstream sn; sn << sec.name << "-SYS" << (j + 1);
            sys.name = sn.str();
            systems_.push_back(sys);
        }
    }
}

std::vector<PCGSector> PCGWorldGen::getSectors() const { return sectors_; }

std::vector<PCGStarSystem> PCGWorldGen::getSystemsInSector(uint64_t sectorId) const
{
    std::vector<PCGStarSystem> result;
    for (const auto& s : systems_)
        if (s.sectorId == sectorId) result.push_back(s);
    return result;
}

PCGStarSystem PCGWorldGen::getSystem(uint64_t systemId) const
{
    for (const auto& s : systems_)
        if (s.systemId == systemId) return s;
    return {};
}

void PCGWorldGen::clear() { sectors_.clear(); systems_.clear(); }

} // namespace NovaForge::Gameplay::PCG
