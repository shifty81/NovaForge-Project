#include "CharacterEditorBridge.h"

#include "Characters/Editor/CharacterEditorSystem.h"

namespace Runtime::Player
{
CharacterEditorBridge::CharacterEditorBridge()
    : m_initialized(false)
    , m_editorOpen(false)
{
}

CharacterEditorBridge::~CharacterEditorBridge() = default;

bool CharacterEditorBridge::Initialize()
{
    // TODO:
    // - acquire CharacterEditorSystem reference from the service locator
    // - register editor mode change callbacks with the runtime controller
    m_initialized = true;
    return true;
}

void CharacterEditorBridge::Shutdown()
{
    // TODO:
    // - close any open editor session
    // - deregister editor mode callbacks
    // - release CharacterEditorSystem reference
    if (m_editorOpen)
    {
        CloseCharacterEditor();
    }
    m_initialized = false;
}

bool CharacterEditorBridge::OpenCharacterEditor(const std::string& characterId)
{
    // TODO:
    // - validate that characterId is a registered, live character
    // - call CharacterEditorSystem::OpenEditor
    // - notify the runtime controller to enter editor mode
    (void)characterId;
    m_activeCharacterId = characterId;
    m_editorOpen = true;
    return true;
}

void CharacterEditorBridge::CloseCharacterEditor()
{
    // TODO:
    // - call CharacterEditorSystem::CloseEditor
    // - notify the runtime controller to exit editor mode
    m_editorOpen = false;
    m_activeCharacterId.clear();
}

bool CharacterEditorBridge::IsEditorOpen() const
{
    return m_editorOpen;
}

void CharacterEditorBridge::ApplyEditorState()
{
    // TODO:
    // - retrieve CharacterEditorSystem::GetState
    // - translate CharacterEditorState fields into live character component updates
    //   (mesh swaps, material overrides, accessory attachment)
    // - broadcast an OnCharacterCustomizationApplied event
}
}
