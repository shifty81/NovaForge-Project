#pragma once

#include <string>

class TaskParserBridge
{
public:
    static std::string NormalizePrompt(const std::string& RawPrompt);
};
