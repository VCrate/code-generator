#pragma once

#include <vcrate/Alias.hpp>

#include <string>

namespace vcrate { namespace code_gen {

struct Type {
    constexpr Type(ui32 size, std::string_view const& name) : size(size), name(name) {}

    ui32 size;
    std::string_view name;
};

namespace type {
    constexpr Type ptr (4, "ptr");
    constexpr Type none (0, "void");

    constexpr Type i32 (4, "i32");
    constexpr Type boolean(1, "bool");
}

}}