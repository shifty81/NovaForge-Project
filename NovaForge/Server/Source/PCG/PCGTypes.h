#pragma once
#include <string>

struct PCGSeed
{
    int Value = 0;
};

struct PCGContext
{
    PCGSeed Seed;
    float Density = 1.0f;
};