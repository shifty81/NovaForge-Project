#pragma once

#include <string>

class DataConversionUtils
{
public:
    static std::string MakeId(const std::string& Text);
    static std::string Quote(const std::string& Text);
};
