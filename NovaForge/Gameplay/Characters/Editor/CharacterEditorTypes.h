#pragma once

#include <string>
#include <vector>

struct CharacterEditorOption
{
    std::string Category;
    std::vector<std::string> Choices;
};

struct CharacterEditorState
{
    std::string CharacterId;
    bool bVisible = false;
    std::string SelectedHelmet;
    std::string SelectedBackpack;
    std::string PrimaryColor = "industrial_gray";
    std::string AccentColor = "signal_orange";
};
