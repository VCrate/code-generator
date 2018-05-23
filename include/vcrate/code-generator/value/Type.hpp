#pragma once

#include <vcrate/Alias.hpp>

#include <string>

namespace vcrate { namespace code_gen {

struct Type {
    Type(ui32 size, std::string const& name) : size(size), name(name) {}

    ui32 size;
    std::string name;
};

namespace type {
    extern Type ptr;
    extern Type none;
    extern Type i32;
    extern Type boolean;
}

}}