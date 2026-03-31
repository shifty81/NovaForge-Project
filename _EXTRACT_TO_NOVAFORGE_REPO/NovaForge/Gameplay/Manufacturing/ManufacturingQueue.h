// ManufacturingQueue.h
// NovaForge manufacturing — job queuing, progress tracking, and output delivery.

#pragma once
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace NovaForge::Gameplay::Manufacturing
{

enum class EJobStatus : uint8_t
{
    Queued,
    InProgress,
    Completed,
    Cancelled
};

struct RecipeIngredient
{
    std::string itemId;
    uint32_t    quantity = 1;
};

struct RecipeDefinition
{
    std::string                    recipeId;
    std::string                    outputItemId;
    uint32_t                       outputQuantity = 1;
    float                          durationSeconds = 10.0f;
    std::vector<RecipeIngredient>  ingredients;
};

struct ManufacturingJob
{
    uint64_t     jobId       = 0;
    uint64_t     stationId   = 0;
    uint64_t     playerId    = 0;
    std::string  recipeId;
    uint32_t     quantity    = 1;
    EJobStatus   status      = EJobStatus::Queued;
    float        progress    = 0.0f; ///< 0-1 completion fraction
};

class ManufacturingQueue
{
public:
    ManufacturingQueue()  = default;
    ~ManufacturingQueue() = default;

    void initialise();
    void shutdown();

    /// Register a recipe definition.
    void registerRecipe(const RecipeDefinition& recipe);

    /// Queue a manufacturing job for a player at a station.
    /// Returns the job ID, or 0 if the recipe is unknown.
    uint64_t queueJob(uint64_t stationId, uint64_t playerId,
                      const std::string& recipeId, uint32_t quantity);

    /// Advance in-progress jobs by dt seconds.
    void tick(float deltaSeconds);

    /// Cancel a queued or in-progress job.
    bool cancelJob(uint64_t jobId);

    /// Retrieve completed jobs for a player and deliver output to inventory.
    std::vector<ManufacturingJob> collectCompleted(uint64_t playerId);

    std::optional<ManufacturingJob> findJob(uint64_t jobId) const;
    std::vector<ManufacturingJob>   listJobs(uint64_t playerId) const;

    const std::vector<RecipeDefinition>& getRecipes() const { return recipes_; }

private:
    std::vector<RecipeDefinition> recipes_;
    std::vector<ManufacturingJob> jobs_;
    uint64_t nextJobId_ = 1;

    const RecipeDefinition* findRecipe(const std::string& recipeId) const;
};

} // namespace NovaForge::Gameplay::Manufacturing
