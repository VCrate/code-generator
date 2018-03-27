#include <vcrate/code-generator/value/Value.hpp>

namespace vcrate { namespace code_gen {

Value::Value(Type const& type, ui32 id) : type(type), id(id) {}

Type Value::get_type() const {
    return type;
}

ui32 Value::get_id() const {
    return id;
}

}}