#include <iostream>

#include <vcrate/Alias.hpp>
#include <vcrate/code-generator/context/Context.hpp>
#include <vcrate/code-generator/function/Function.hpp>
#include <vcrate/code-generator/helper/Dumper.hpp>
#include <vcrate/code-generator/instruction/Application.hpp>

#include <vcrate/vcx/Executable.hpp>

int main() {
    using namespace vcrate;
    using namespace vcrate::code_gen;

    Context ctx;

    // ui32, ui8... -> void*
    Signature func_signature(type::void_ptr);
    func_signature.add_arg(type::uint32);
    func_signature.enable_variable_args(type::uint8);

    Function& func = ctx.create_function("alloc_filled", func_signature);
    auto ret = func.create_value(type::void_ptr);
    func.retrn(ret);

    //CompiledContext cc = ctx.compile();
    ctx.dump(std::cout);
    //cc.dump(std::cout);

    Application _ret = app_return(std::vector<Value>{ ret });

    std::cout << operation::retrn.name << ", " << _ret.to_string() << '\n';

    auto r = make_application(operation::retrn, { Access::Read });

    Application __ret = r(std::vector<Value>{ ret });
    std::cout << __ret.to_string() << '\n';
    return 0;
}