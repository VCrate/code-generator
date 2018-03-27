#pragma once

#include <vcrate/Alias.hpp>
#include <vcrate/code-generator/value/Type.hpp>

namespace vcrate { namespace code_gen {

class Value {
public:

    Value(Type const& type, ui32 id);

    Type get_type() const;
    ui32 get_id() const;

private:

    Type type;
    ui32 id;

};

}}