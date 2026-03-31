#include "Gameplay/Characters/Editor/CharacterEditorSystem.h"
#include <iostream>

bool CharacterEditorSystem::Initialize()
{
    std::cout << "[CharacterEditor] Initialize\n";
    return true;
}

void CharacterEditorSystem::OpenEditor(const std::string& CharacterId)
{
    State.CharacterId = CharacterId;
    State.bVisible = true;
    std::cout << "[CharacterEditor] Opened for " << CharacterId << "\n";
}

void CharacterEditorSystem::CloseEditor()
{
    State.bVisible = false;
    std::cout << "[CharacterEditor] Closed\n";
}

void CharacterEditorSystem::SetHelmet(const std::string& HelmetId)
{
    State.SelectedHelmet = HelmetId;
}

void CharacterEditorSystem::SetBackpack(const std::string& BackpackId)
{
    State.SelectedBackpack = BackpackId;
}

void CharacterEditorSystem::SetPrimaryColor(const std::string& ColorId)
{
    State.PrimaryColor = ColorId;
}

void CharacterEditorSystem::SetAccentColor(const std::string& ColorId)
{
    State.AccentColor = ColorId;
}

const CharacterEditorState& CharacterEditorSystem::GetState() const
{
    return State;
}
