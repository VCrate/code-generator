#include <iostream>
#include <fstream>

#include <vcrate/Alias.hpp>
#include <vcrate/code-generator/helper/Dumper.hpp>
#include <vcrate/code-generator/core/Function.hpp>
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
    b.insn_compare(p0, constant_0);
    b.end_with_branch_eq(allocate, error);
*/

    Block& a = func.get_block();
    Block b, c, d, e, f;

    a.name = "Block_A";
    b.name = "Block_B";
    c.name = "Block_C";
    d.name = "Block_D";
    e.name = "Block_E";
    f.name = "Block_F";
/*
    auto fake = a.create_value(type::i32);
    d.end_with_return(fake);
    //d.end_with_jump(e);
    c.end_with_branch_gt_eq(e, d);
    b.end_with_jump(c);
    e.end_with_jump(b);

    a.end_with_branch_eq(f, b);
    f.end_with_halt(fake);
*/

    auto const_1 = a.create_value(type::i32);
    auto value = a.create_value(type::i32);
    a.insn_load(const_1, { 1, 0, 0, 0 });
    a.insn_copy(value, const_1);
    a.insn_dbg(value);
    a.end_with_halt(value);

    /*
        MOV %A, 0
        MOV %B, %A
        MOV %B, %A
        HLT
    or
        MOV %B, 0
        MOV %A, %B
        HLT
    or
        MOV %A, 0
        HLT
    */

    auto exe = func.compile();

    if (!exe) {
        std::cout << "Couldn't compile\n";
        return 1;
    }

    exe->dump();
    std::ofstream os("temp/test.vcx");
    os << *exe;

    return 0;
}