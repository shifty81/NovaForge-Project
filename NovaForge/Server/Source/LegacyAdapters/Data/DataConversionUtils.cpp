#include "LegacyAdapters/Data/DataConversionUtils.h"
#include <algorithm>
#include <cctype>

std::string DataConversionUtils::MakeId(const std::string& Text)
{
    std::string Out = Text;
    std::transform(Out.begin(), Out.end(), Out.begin(), [](unsigned char Ch)
    {
        if (std::isalnum(Ch))
        {
            return static_cast<char>(std::tolower(Ch));
        }
        return '_';
    });

    while (Out.find("__") != std::string::npos)
    {
        Out.replace(Out.find("__"), 2, "_");
    }

    return Out;
}

std::string DataConversionUtils::Quote(const std::string& Text)
{
    return "\"" + Text + "\"";
}
