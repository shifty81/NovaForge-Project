#pragma once

#include <string>

namespace Runtime::Player
{
class CharacterEditorBridge
{
public:
    CharacterEditorBridge();
    ~CharacterEditorBridge();

    bool Initialize();
    void Shutdown();

    bool OpenCharacterEditor(const std::string& characterId);
    void CloseCharacterEditor();
    bool IsEditorOpen() const;
    void ApplyEditorState();

private:
    bool        m_initialized;
    bool        m_editorOpen;
    std::string m_activeCharacterId;
};
}
