#pragma once

#include <vcrate/Alias.hpp>

#include <utility>
#include <optional>

namespace vcrate { namespace code_gen {

class Operand {
public:

    static Operand Register(ui32 id);
    static Operand Deferred(ui32 id);
    static Operand Displacement(ui32 id, i32 disp);
    static Operand Value(i32 value);
    static Operand Address(ui32 addr);

    std::pair<ui32, std::optional<ui32>> to_12() const { return b12; }
    std::pair<ui32, std::optional<ui32>> to_24() const { return b24; }

private:

    std::pair<ui32, std::optional<ui32>> b12;
    std::pair<ui32, std::optional<ui32>> b24;

};

}}