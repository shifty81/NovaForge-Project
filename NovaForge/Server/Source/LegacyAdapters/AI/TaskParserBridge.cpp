#include "LegacyAdapters/AI/TaskParserBridge.h"

std::string TaskParserBridge::NormalizePrompt(const std::string& RawPrompt)
{
    if (RawPrompt.empty())
    {
        return "No task prompt supplied.";
    }

    return RawPrompt;
}
