// NovaForgeProjectContext.cpp
// Implementation of the manifest-driven project context.

#include "NovaForgeProjectContext.h"

#include <stdexcept>

namespace NovaForge::App
{

// ============================================================
// Construction
// ============================================================

NovaForgeProjectContext::NovaForgeProjectContext(const ProjectContextConfig& config)
    : m_config(config)
{
}

NovaForgeProjectContext NovaForgeProjectContext::createDefault(
    const std::string& repoRoot)
{
    if (repoRoot.empty())
        throw std::invalid_argument(
            "NovaForgeProjectContext::createDefault: repoRoot must not be empty");

    ProjectContextConfig cfg;
    cfg.repoRoot = repoRoot;
    return NovaForgeProjectContext(cfg);
}

// ============================================================
// Project identity
// ============================================================

const std::string& NovaForgeProjectContext::projectId()   const { return m_config.projectId; }
const std::string& NovaForgeProjectContext::displayName() const { return m_config.displayName; }
const std::string& NovaForgeProjectContext::version()     const { return m_config.version; }

// ============================================================
// Absolute path accessors
// ============================================================

std::string NovaForgeProjectContext::repoRoot()    const
{
    return m_config.repoRoot;
}

std::string NovaForgeProjectContext::dataRoot()    const
{
    return joinPath(m_config.repoRoot, m_config.dataRoot);
}

std::string NovaForgeProjectContext::contentRoot() const
{
    return joinPath(m_config.repoRoot, m_config.contentRoot);
}

std::string NovaForgeProjectContext::docsRoot()    const
{
    return joinPath(m_config.repoRoot, m_config.docsRoot);
}

std::string NovaForgeProjectContext::scriptsRoot() const
{
    return joinPath(m_config.repoRoot, m_config.scriptsRoot);
}

std::string NovaForgeProjectContext::testsRoot()   const
{
    return joinPath(m_config.repoRoot, m_config.testsRoot);
}

std::string NovaForgeProjectContext::engineRoot()  const
{
    return joinPath(m_config.repoRoot, m_config.engineRoot);
}

std::string NovaForgeProjectContext::gameRoot()    const
{
    return joinPath(m_config.repoRoot, m_config.gameRoot);
}

// ============================================================
// Environment flags
// ============================================================

bool NovaForgeProjectContext::isEditorMode() const { return m_config.editorMode; }
bool NovaForgeProjectContext::isDebugMode()  const { return m_config.debugMode;  }

// ============================================================
// Validation
// ============================================================

bool NovaForgeProjectContext::isValid() const
{
    return !m_config.repoRoot.empty()
        && !m_config.projectId.empty()
        && !m_config.version.empty();
}

// ============================================================
// Path helpers
// ============================================================

std::string NovaForgeProjectContext::joinPath(const std::string& base,
                                              const std::string& relative)
{
    if (relative.empty())
        return base;

    // Treat paths starting with '/' (POSIX absolute) or a drive letter followed
    // by ':' (Windows absolute, e.g. "C:\path") as already-absolute.
    // UNC paths (\\server\share) are also treated as absolute via the '\\' check.
    // On this project all repo-internal paths use forward slashes (POSIX convention).
    bool isAbsolute = (!relative.empty() && relative[0] == '/')
                   || (!relative.empty() && relative[0] == '\\')
                   || (relative.size() >= 2 && relative[1] == ':');

    if (isAbsolute)
        return relative;

    if (!base.empty() && (base.back() == '/' || base.back() == '\\'))
        return base + relative;

    return base + '/' + relative;
}

} // namespace NovaForge::App
