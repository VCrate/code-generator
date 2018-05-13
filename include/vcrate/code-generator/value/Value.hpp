#pragma once

#include <vcrate/Alias.hpp>
#include <vcrate/code-generator/value/Type.hpp>

namespace vcrate { namespace code_gen {

struct Value {
    ui64 id;
    Type type;
};

}}