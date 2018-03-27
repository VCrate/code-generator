#pragma once

#include <vcrate/Alias.hpp>
#include <vcrate/code-generator/value/Type.hpp>

#include <variant>

namespace vcrate { namespace code_gen {

class Constant {
public:

    using values_t = std::variant<ui8, i8, ui16, i16, ui32, i32, ui64, i64, std::string>;

    Constant(Type const& type, values_t const& value);

    Type get_type() const;
    values_t get_value() const;

private:

    Type type;
    values_t value;

};

}}