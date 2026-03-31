#pragma once

#include "Gameplay/Characters/Editor/CharacterEditorTypes.h"
#include <string>

class CharacterEditorSystem
{
public:
    bool Initialize();
    void OpenEditor(const std::string& CharacterId);
    void CloseEditor();
    void SetHelmet(const std::string& HelmetId);
    void SetBackpack(const std::string& BackpackId);
    void SetPrimaryColor(const std::string& ColorId);
    void SetAccentColor(const std::string& ColorId);
    const CharacterEditorState& GetState() const;

private:
    CharacterEditorState State;
};
