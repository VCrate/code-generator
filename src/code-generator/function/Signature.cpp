#include <vcrate/code-generator/function/Signature.hpp>

namespace vcrate { namespace code_gen {

Signature::Signature(Type const& return_type) : ret(return_type) {}

void Signature::set_return(Type const& type) {
    ret = type;
}

void Signature::add_arg(Type const& type) {
    args.emplace_back(type);
}

void Signature::enable_variable_args(Type const& type) {
    variable_args = type;
}

void Signature::disable_variable_args() {
    variable_args = std::nullopt;
}

Type Signature::get_return() const {
    return ret;
}

Type Signature::get_arg(ui32 pos) const {
    return args[pos];
}

ui32 Signature::get_arg_count() const {
    return args.size();
}

std::optional<Type> Signature::get_variable_args() const {
    return variable_args;
}

}}