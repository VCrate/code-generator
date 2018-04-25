#pragma once

#include <vcrate/Alias.hpp>
#include <vcrate/code-generator/value/Type.hpp>

namespace vcrate { namespace code_gen {

struct Value {
    const ui64 id;
    const Type type;
};

}}