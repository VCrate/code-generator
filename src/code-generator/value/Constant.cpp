#include <vcrate/code-generator/value/Constant.hpp>

namespace vcrate { namespace code_gen {

Constant::Constant(Type const& type, Constant::values_t const& value) : type(type), value(value) {}

Type Constant::get_type() const {
    return type;
}

Constant::values_t Constant::get_value() const {
    return value;
}

}}