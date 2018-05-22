#pragma once

#include <vcrate/Alias.hpp>
#include <vcrate/code-generator/value/Type.hpp>

namespace vcrate { namespace code_gen {

class Context;

class Value {
public:

    Value(Context& context, ui64 id, Type const& type);

    Context& get_context();
    ui64 get_id() const;
    Type get_type() const;

private:

    Context* context;
    ui64 id;
    Type type;
};

}}