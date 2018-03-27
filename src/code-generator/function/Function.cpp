#include <vcrate/code-generator/function/Function.hpp>

#include <vcrate/code-generator/context/Context.hpp>
#include <vcrate/code-generator/helper/Dumper.hpp>

namespace vcrate { namespace code_gen {


Function::Function(Context& ctx, std::string const& name, Signature const& signature) 
    : ctx(&ctx), name(name), signature(signature), last_id(signature.get_arg_count()) {}

Constant& Function::create_constant(Type const& type, Constant::values_t const& value) {
    return ctx->create_constant(type, value);
}

Value Function::create_value(Type const& type) {
    return Value { type, last_id++ };
}

void Function::dump(std::ostream& os) const {
    os << Dumper::bolder() << Dumper::colorizer(Color::Blue) << name << ": " << Dumper::clearer();
    os << "( " << Dumper::colorizer(Color::Cyan);
    for(ui32 i = 0; i < signature.get_arg_count(); ++i) {
        if (i > 0)
            os << ", ";
        os << signature.get_arg(i).name;
    }
    if (auto var = signature.get_variable_args(); var) {
        if (signature.get_arg_count() > 0)
            os << ", ";
        os << var->name << "...";
    }
    os << Dumper::clearer() << " ) -> " << Dumper::colorizer(Color::Magenta) << signature.get_return().name;
    os << Dumper::clearer() << '\n';
    for(auto const& is : instructions) {
        os << '\t';
        is.dump(os);
        os << '\n';
    }
    os << Dumper::clearer() << '\n';
}

void Function::retrn(Value const& value) {
    instructions.emplace_back(Instruction {
        operation::retrn,
        {
            value
        }
    });
}

}}