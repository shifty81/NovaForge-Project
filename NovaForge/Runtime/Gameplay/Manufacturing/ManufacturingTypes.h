#pragma once

#include <string>
#include <vector>

struct ManufacturingJob
{
    std::string JobId;
    std::string RecipeId;
    float Progress = 0.0f;
    float RequiredTime = 10.0f;
    bool bCompleted = false;
};

struct ManufacturingQueue
{
    std::string QueueId;
    std::vector<ManufacturingJob> Jobs;
};
