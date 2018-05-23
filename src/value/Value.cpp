#include <vcrate/code-generator/value/Value.hpp>

namespace vcrate { namespace code_gen {

Value::Value(Context& context, ui64 id, Type const& type) : context(&context), id(id), type(type) {}

Context& Value::get_context() {
    return *context;
}

ui64 Value::get_id() const {
    return id;
}

Type Value::get_type() const {
    return type;
}


}}