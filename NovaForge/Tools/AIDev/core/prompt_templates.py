"""
NovaForge Dev AI — Language-Aware Prompt Templates (Phase 2)

Provides subsystem-specific prompt templates so the LLM receives
context tailored to the language and domain being worked on.

Each template is a short preamble prepended to the user's prompt
before it reaches the LLM.  The template reminds the model of the
relevant conventions, file patterns, and APIs for that subsystem.

Usage:
    from core.prompt_templates import PromptTemplates

    templates = PromptTemplates()
    preamble = templates.for_subsystem("cpp_server")
    full_prompt = preamble + "\\n\\n" + user_prompt
"""

import logging
from typing import Optional

log = logging.getLogger(__name__)


# ---------------------------------------------------------------------------
# Template definitions — one per language / subsystem
# ---------------------------------------------------------------------------

_TEMPLATES: dict[str, str] = {
    # ── C++ Server ────────────────────────────────────────────────────
    "cpp_server": (
        "You are editing C++ code in the **cpp_server/** subsystem.\n"
        "Conventions:\n"
        "- snake_case filenames, methods, and local variables.\n"
        "- Trailing snake_case_ for member variables.\n"
        "- namespace atlas { namespace systems { ... } }\n"
        "- ECS systems extend SingleComponentSystem<Component>.\n"
        "- All mutating methods return bool (true = success).\n"
        "- All query methods return a safe default when entity is missing.\n"
        "- Use atlas::Logger — never std::cout / std::cerr.\n"
        "- Include guards: #ifndef NOVAFORGE_SYSTEMS_<NAME>_H\n"
        "- Include order: own header → project → third-party → stdlib.\n"
    ),

    # ── C++ Client ────────────────────────────────────────────────────
    "cpp_client": (
        "You are editing C++ code in the **cpp_client/** subsystem.\n"
        "Conventions:\n"
        "- snake_case filenames, methods, and local variables.\n"
        "- Trailing snake_case_ for member variables.\n"
        "- namespace atlas { ... }\n"
        "- OpenGL rendering with GLFW; camera uses -Z forward at yaw=0.\n"
        "- WORLD_UP=(0,1,0), WORLD_FORWARD=(0,0,-1), WORLD_RIGHT=(1,0,0).\n"
        "- Include guards: #ifndef NOVAFORGE_CLIENT_<NAME>_H\n"
    ),

    # ── Atlas Engine ──────────────────────────────────────────────────
    "engine": (
        "You are editing C++ code in the **engine/** subsystem (Atlas Engine).\n"
        "Conventions:\n"
        "- PascalCase filenames and methods.\n"
        "- m_camelCase for member variables.\n"
        "- namespace atlas { ... }\n"
        "- Core modules: ECS, rendering (OpenGL), audio, PCG.\n"
        "- Engine camera uses +Z forward.\n"
    ),

    # ── Atlas Editor ──────────────────────────────────────────────────
    "editor": (
        "You are editing C++ code in the **editor/** subsystem (Atlas Editor).\n"
        "Conventions:\n"
        "- PascalCase filenames and methods.\n"
        "- m_camelCase for member variables.\n"
        "- namespace atlas { ... }\n"
        "- Editor tools register via EditorCommandBus.\n"
        "- Changes are undoable through UndoableCommandBus.\n"
        "- DeltaEditStore for structural edits.\n"
    ),

    # ── Python (AI tools, scripts) ────────────────────────────────────
    "python": (
        "You are editing Python code in **ai_dev/** or **tools/**.\n"
        "Conventions:\n"
        "- Python 3.8+ compatible.\n"
        "- Use logging module (log = logging.getLogger(__name__)).\n"
        "- Type hints for public API methods.\n"
        "- Classes: PascalCase. Functions/variables: snake_case.\n"
        "- Tests use unittest + unittest.mock.\n"
    ),

    # ── Lua (game scripts) ────────────────────────────────────────────
    "lua": (
        "You are editing Lua scripts used by the Atlas Engine.\n"
        "Conventions:\n"
        "- Use local variables wherever possible.\n"
        "- Scripts are reloaded at runtime via dofile().\n"
        "- No global state pollution — wrap in local tables.\n"
        "- Use atlas.log() for logging, not print().\n"
    ),

    # ── GLSL (shaders) ────────────────────────────────────────────────
    "glsl": (
        "You are editing GLSL shaders for the Atlas Engine renderer.\n"
        "Conventions:\n"
        "- GLSL 4.50 core profile.\n"
        "- Vertex shaders: .vert extension.\n"
        "- Fragment shaders: .frag extension.\n"
        "- Use uniform buffer objects (UBOs) for camera/model matrices.\n"
        "- Validate with: glslangValidator -V <shader>\n"
    ),

    # ── C# (.NET) ─────────────────────────────────────────────────────
    "csharp": (
        "You are editing C# / .NET code.\n"
        "Conventions:\n"
        "- PascalCase for classes, methods, and properties.\n"
        "- camelCase for local variables and parameters.\n"
        "- _camelCase for private fields.\n"
        "- Build with: dotnet build\n"
        "- Test with: dotnet test\n"
    ),

    # ── JSON (data files) ─────────────────────────────────────────────
    "json": (
        "You are editing JSON data files.\n"
        "Conventions:\n"
        "- Use consistent indentation (2 spaces).\n"
        "- Keys in snake_case.\n"
        "- Files are hot-reloadable by the game server.\n"
    ),

    # ── Blender (PCG pipeline) ────────────────────────────────────────
    "blender": (
        "You are editing Blender Python scripts for the PCG pipeline.\n"
        "Conventions:\n"
        "- Target Blender 5.0+ API.\n"
        "- Use bmesh for procedural geometry.\n"
        "- Export as .glb or .fbx.\n"
        "- Extension manifest: blender_manifest.toml.\n"
        "- Run headless: blender --background --python <script>\n"
    ),

    # ── CMake ─────────────────────────────────────────────────────────
    "cmake": (
        "You are editing CMakeLists.txt build configuration.\n"
        "Conventions:\n"
        "- Minimum CMake 3.16.\n"
        "- Server sources listed in CORE_SOURCES.\n"
        "- Test files auto-discovered via GLOB.\n"
        "- Use -DBUILD_TESTS=ON -DUSE_STEAM_SDK=OFF for dev builds.\n"
    ),
}


# ---------------------------------------------------------------------------
# File extension → subsystem mapping
# ---------------------------------------------------------------------------

_EXT_TO_SUBSYSTEM: dict[str, str] = {
    ".cpp": "cpp_server",  # default; refined by path
    ".h": "cpp_server",
    ".hpp": "cpp_server",
    ".py": "python",
    ".lua": "lua",
    ".vert": "glsl",
    ".frag": "glsl",
    ".glsl": "glsl",
    ".cs": "csharp",
    ".json": "json",
    ".cmake": "cmake",
}

# Path prefix → subsystem (takes priority over extension mapping)
_PATH_PREFIX_TO_SUBSYSTEM: list[tuple[str, str]] = [
    ("cpp_server/", "cpp_server"),
    ("cpp_client/", "cpp_client"),
    ("engine/", "engine"),
    ("editor/", "editor"),
    ("ai_dev/", "python"),
    ("tools/Blender", "blender"),
]


class PromptTemplates:
    """
    Selects and returns the best prompt preamble for a given subsystem,
    file path, or file extension.
    """

    def for_subsystem(self, subsystem: str) -> str:
        """
        Return the prompt preamble for a named subsystem.

        Args:
            subsystem: One of the keys in _TEMPLATES (e.g. "cpp_server",
                       "engine", "python", "glsl", "csharp").

        Returns:
            The template string, or a generic fallback if unknown.
        """
        return _TEMPLATES.get(subsystem, "")

    def for_file(self, rel_path: str) -> str:
        """
        Infer the subsystem from a file path and return the matching template.

        Checks path prefixes first, then falls back to file extension.
        """
        subsystem = self.detect_subsystem(rel_path)
        return self.for_subsystem(subsystem)

    def detect_subsystem(self, rel_path: str) -> str:
        """
        Detect the subsystem from a relative file path.

        Returns the subsystem key (e.g. "cpp_server", "engine", "python").
        """
        # Check path prefixes first (most specific)
        for prefix, subsystem in _PATH_PREFIX_TO_SUBSYSTEM:
            if rel_path.startswith(prefix):
                return subsystem

        # Fall back to extension
        dot = rel_path.rfind(".")
        if dot >= 0:
            ext = rel_path[dot:].lower()
            return _EXT_TO_SUBSYSTEM.get(ext, "")

        return ""

    def available_subsystems(self) -> list[str]:
        """Return a sorted list of all registered subsystem names."""
        return sorted(_TEMPLATES.keys())

    def enrich_prompt(self, prompt: str, rel_path: Optional[str] = None,
                      subsystem: Optional[str] = None) -> str:
        """
        Prepend the appropriate template preamble to a user prompt.

        Args:
            prompt:     The user's raw prompt text.
            rel_path:   Optional file path to auto-detect subsystem.
            subsystem:  Explicit subsystem override.

        Returns:
            The enriched prompt with preamble prepended, or the original
            prompt if no matching template is found.
        """
        if subsystem:
            preamble = self.for_subsystem(subsystem)
        elif rel_path:
            preamble = self.for_file(rel_path)
        else:
            preamble = ""

        if preamble:
            return f"{preamble}\n---\n\n{prompt}"
        return prompt
