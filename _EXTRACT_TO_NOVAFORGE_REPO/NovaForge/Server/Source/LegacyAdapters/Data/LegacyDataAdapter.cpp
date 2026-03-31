#include "LegacyAdapters/Data/LegacyDataAdapter.h"
#include "LegacyAdapters/Data/DataConversionUtils.h"
#include <sstream>

const char* LegacyDataAdapter::GetAdapterName() const
{
    return "LegacyDataAdapter";
}

AdapterReport LegacyDataAdapter::ValidateSource(const LegacySourceInfo& Source) const
{
    AdapterReport Report;
    Report.bSuccess = (Source.Origin != ELegacyOrigin::Unknown);
    Report.Summary = Report.bSuccess
        ? "Legacy data source accepted for conversion."
        : "Legacy data source origin is unknown.";
    return Report;
}

AdapterReport LegacyDataAdapter::Convert(const std::string& RawInput, std::string& OutConverted) const
{
    // Minimal bridge format:
    // item:<Name>:<Category>:<Value>:<StackSize>
    std::stringstream In(RawInput);
    std::string Type;
    std::getline(In, Type, ':');

    if (Type == "item")
    {
        std::string Name, Category, Value, Stack;
        std::getline(In, Name, ':');
        std::getline(In, Category, ':');
        std::getline(In, Value, ':');
        std::getline(In, Stack, ':');

        std::ostringstream Out;
        Out << "{\n"
            << "  \"id\": " << DataConversionUtils::Quote(DataConversionUtils::MakeId(Name)) << ",\n"
            << "  \"name\": " << DataConversionUtils::Quote(Name) << ",\n"
            << "  \"category\": " << DataConversionUtils::Quote(Category) << ",\n"
            << "  \"value\": " << Value << ",\n"
            << "  \"stack_size\": " << Stack << "\n"
            << "}";
        OutConverted = Out.str();

        return {true, "Converted legacy item to Master Repo item definition.", {}};
    }

    if (Type == "module")
    {
        std::string Name, Category, SX, SY, SZ;
        std::getline(In, Name, ':');
        std::getline(In, Category, ':');
        std::getline(In, SX, ':');
        std::getline(In, SY, ':');
        std::getline(In, SZ, ':');

        std::ostringstream Out;
        Out << "{\n"
            << "  \"id\": " << DataConversionUtils::Quote(DataConversionUtils::MakeId(Name)) << ",\n"
            << "  \"name\": " << DataConversionUtils::Quote(Name) << ",\n"
            << "  \"category\": " << DataConversionUtils::Quote(Category) << ",\n"
            << "  \"size\": {\n"
            << "    \"x\": " << SX << ",\n"
            << "    \"y\": " << SY << ",\n"
            << "    \"z\": " << SZ << "\n"
            << "  }\n"
            << "}";
        OutConverted = Out.str();

        return {true, "Converted legacy module to Master Repo module definition.", {}};
    }

    return {false, "Unsupported legacy data record type.", {"Expected prefixes: item or module"}};
}
