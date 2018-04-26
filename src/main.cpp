#include <iostream>

#include <vcrate/Alias.hpp>
#include <vcrate/code-generator/helper/Dumper.hpp>
#include <vcrate/code-generator/core/Function.hpp>
#include <vcrate/code-generator/core/CompiledFunction.hpp>
#include <vcrate/code-generator/core/Block.hpp>
#include <vcrate/code-generator/value/Value.hpp>
#include <vcrate/code-generator/value/Type.hpp>

#include <vcrate/vcx/Executable.hpp>

int main() {
    using namespace vcrate;
    using namespace vcrate::code_gen;


    Function func;
    // func.set_return_type(type::ptr);
    // func.set_parameters(type::ui32);
    /*
    Block& b = func.get_block();
    Value p0 = func.get_parameter(0);

    Block error;
    Block allocate;

    Value ex = error.create_value(type::i32);
    error.insn_store(ex, 0);
    error.end_with_throw(ex);
    
    Value allocated_block = allocate.insn_new(p0);
    allocate.end_with_return(allocated_block);

    Value constant_0 = b.create_value(type::i32);
    b.insn_store(constant_0, 0);
    Value inf_eq_0 = b.insn_inf_eq(p0, constant_0);
    b.end_with_branch(inf_eq_0, 
        allocate,
        error);
*/

    Value fake { 0, type::ptr };

    Block& a = func.get_block();
    Block b, c, d, e, f;

    a.name = "Block_A";
    b.name = "Block_B";
    c.name = "Block_C";
    d.name = "Block_D";
    e.name = "Block_E";
    f.name = "Block_F";

    //d.end_with_return(fake);
    d.end_with_jump(b);
    c.end_with_branch(fake, e, d);
    b.end_with_jump(c);
    e.end_with_jump(b);

    a.end_with_branch(fake, f, b);
    f.end_with_halt(fake);

    std::cout << "Terminate ? " << func.check_terminate() << '\n';
    CompiledFunction::from_function(func);

    return 0;
}