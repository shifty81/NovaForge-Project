// NovaForgeProjectContext.h
// Isolates all project-level path, metadata, and config knowledge from gameplay code.
//
// Epic 5 / Task 5.2 — Isolated project context
// Epic 5 / Task 5.4 — Manifest-driven path resolution
//
// Rules:
// - This class must not include gameplay, engine, or AtlasAI headers.
// - All paths are resolved relative to the provided repoRoot at construction.
// - Callers should treat the context as read-only after initialisation.

#pragma once

#include <string>

namespace NovaForge::App
{

// ============================================================
// Configuration struct (populated by bootstrap from manifest/env)
// ============================================================

struct ProjectContextConfig
{
    // Project identity
    std::string projectId    = "novaforge";
    std::string displayName  = "NovaForge";
    std::string version      = "0.1.0";

    // Root path — absolute path to the repository root
    std::string repoRoot;

    // Relative sub-paths (relative to repoRoot)
    std::string dataRoot     = "NovaForge/Data";
    std::string contentRoot  = "NovaForge/Content";
    std::string docsRoot     = "Docs";
    std::string scriptsRoot  = "Scripts";
    std::string testsRoot    = "Tests";
    std::string engineRoot   = "Atlas";
    std::string gameRoot     = "NovaForge";

    // Environment flags
    bool        editorMode   = false;
    bool        debugMode    = true;
};

// ============================================================
// NovaForgeProjectContext
// ============================================================

class NovaForgeProjectContext
{
public:
    /// Constructs a context from an explicit config struct.
    explicit NovaForgeProjectContext(const ProjectContextConfig& config);

    /// Creates a context populated with the default NovaForge layout.
    /// The repoRoot must be an absolute path.
    static NovaForgeProjectContext createDefault(const std::string& repoRoot);

    // --------------------------------------------------------
    // Project identity
    // --------------------------------------------------------
    const std::string& projectId()   const;
    const std::string& displayName() const;
    const std::string& version()     const;

    // --------------------------------------------------------
    // Absolute path accessors
    // All paths are fully resolved (repoRoot + relative sub-path).
    // --------------------------------------------------------
    std::string repoRoot()     const;
    std::string dataRoot()     const;
    std::string contentRoot()  const;
    std::string docsRoot()     const;
    std::string scriptsRoot()  const;
    std::string testsRoot()    const;
    std::string engineRoot()   const;
    std::string gameRoot()     const;

    // --------------------------------------------------------
    // Environment flags
    // --------------------------------------------------------
    bool isEditorMode() const;
    bool isDebugMode()  const;

    // --------------------------------------------------------
    // Validation
    // --------------------------------------------------------
    /// Returns true when the repoRoot is non-empty and the config is sane.
    bool isValid() const;

private:
    ProjectContextConfig m_config;

    static std::string joinPath(const std::string& base,
                                const std::string& relative);
};

} // namespace NovaForge::App
