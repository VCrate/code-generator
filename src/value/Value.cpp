#include <vcrate/code-generator/value/Value.hpp>

namespace vcrate { namespace code_gen {

Value::Value(Context& context, Type const& type) : context(&context), type(type) {}

Context& Value::get_context() {
    return *context;
}

Type Value::get_type() const {
    return type;
}


}}