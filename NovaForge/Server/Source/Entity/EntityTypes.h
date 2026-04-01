#pragma once

#include <cstdint>
#include <string>

struct EntityId
{
    std::uint64_t Value = 0;

    bool IsValid() const { return Value != 0; }
    std::string ToString() const { return std::to_string(Value); }
};
