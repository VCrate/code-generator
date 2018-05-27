#pragma once

#include <vcrate/Alias.hpp>
#include <vcrate/code-generator/value/Type.hpp>

namespace vcrate { namespace code_gen {

class Context;

class Value {
public:

    Context& get_context();
    Type get_type() const;

    std::string name;

private:

    friend class Context;

    Value(Context& context, Type const& type);

    Context* context;
    Type type;
};

}}