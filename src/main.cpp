#include <iostream>
#include <fstream>

#include <vcrate/Alias.hpp>
#include <vcrate/code-generator/helper/Dumper.hpp>
#include <vcrate/code-generator/core/Context.hpp>
#include <vcrate/code-generator/core/Function.hpp>
#include <vcrate/code-generator/core/Block.hpp>
#include <vcrate/code-generator/value/Value.hpp>
#include <vcrate/code-generator/value/Type.hpp>

#include <vcrate/vcx/Executable.hpp>

int main() {
    using namespace vcrate;
    using namespace vcrate::code_gen;


    Context ctx;
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

    Function& func = ctx.create_function("Function_A");
    Block& a = func.get_block();
    Block &b(ctx.create_block("Block_B")), 
          &c(ctx.create_block("Block_C")), 
          &d(ctx.create_block("Block_D")), 
          &e(ctx.create_block("Block_E")), 
          &f(ctx.create_block("Block_F"));

    a.name = "Block_A";
/*
    auto fake = ctx.create_value(type::i32);
    d.end_with_return(fake);
    //d.end_with_jump(e);
    c.end_with_branch_gt_eq(e, d);
    b.end_with_jump(c);
    e.end_with_jump(b);

    a.end_with_branch_eq(f, b);
    f.end_with_halt(fake);
*/

    auto& const_1 = ctx.create_value(type::i32, "const_1");
    auto& const_2 = ctx.create_value(type::i32, "const_2");
    auto& const_3 = ctx.create_value(type::i32, "const_3");
    auto& value = ctx.create_value(type::i32, "value");
    a.insn_load(const_1, { 1, 0, 0, 0 });
    a.insn_load(const_2, { 2, 0, 0, 0 });
    a.insn_load(const_3, { 3, 0, 0, 0 });
    a.insn_copy(value, const_1);
    a.insn_dbg(value);
    a.insn_compare(const_1, const_2);
    a.end_with_branch_eq(b, c);

    b.insn_dbg(const_2);
    b.end_with_halt(value);

    c.insn_dbg(const_3);
    c.end_with_jump(d);

    d.insn_dbg(value);
    d.end_with_return();

    a.dump();
    b.dump();
    c.dump();
    d.dump();

    auto exe = ctx.compile();

    if (!exe) {
        std::cout << "Couldn't compile\n";
        return 1;
    }

    exe->dump();
    std::ofstream os("temp/test.vcx");
    os << *exe;

    return 0;
}