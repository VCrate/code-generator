#include <iostream>

#include <vcrate/Alias.hpp>

int main() {
    using namespace vcrate;
    using namespace vcrate::code_gen;

    Context ctx;

    // ui32, ui8... -> void*
    Signature func_signature(Type::ptr);
    func_signature.add_arg(Type::uinteger32);
    func_signature.enable_var_args(Type::uinteger32);

    auto func = ctx.create_function("alloc_filled", func_signature);
    /*
        alloc_filled :: ui32 size, ui8... args -> void*
            void* ret = allocate(size)
            ui32 counter = 0
            while counter < args.size()
                ret[counter] = args[counter]
                ++counter
            ui32 res = print("allocated")
            return ret
    */
    /*      Expected
     ******************
     * %A is the 'size' argument
     * variables arguments are on the stack, then the number of argument 
     * like ...|10|20|30|40|50| 5|
     * return value in %A
     ******************
     *  NEW %A, %A
     *  MOV %B, 0
     *  MOV %C, [%SP - 4] // args count
     *  CMP %B, %C
     *  JMPGE for_end
     * for_begin:
     *  MOV %D, %A
     *  ADD %D, %C
     *  LEA %E, [%SP - 4]
     *  SUB %E, %B
     *  ADD %E, %C
     *  MOV [%D], [%E]
     *  INC %B
     *  JMP for_begin
     * for_end:
     *  PUSH %A
     *  MOV %A, [jmp_table_to_data_allocated_str]
     *  SYM %B, [jmp_table_to_data_print_str]
     *  CALL %B //ret in %A
     *  POP %A
     *  ret
    */
    /*      Minimal Expected
     ******************
     * stack : size, varaible_args, args count
     * return value in %A
     ******************
     *  LEA %B, %SP
     *  SUB %B, [%SP - 4]
     *  SUB %B, 4
     *  NEW %A, %B
     *  MOV %C, 0
     *  MOV %D, [%SP - 4] // args count
     *  CMP %C, %D
     *  JMPGE for_end
     * for_begin:
     *  MOV %E, %A
     *  ADD %E, %D
     *  LEA %F, [%SP - 4]
     *  SUB %F, %C
     *  ADD %F, %D
     *  MOV [%E], [%F]
     *  INC %D
     *  JMP for_begin
     * for_end:
     *  PUSH %A
     *  MOV %A, [jmp_table_to_data_allocated_str]
     *  SYM %G, [jmp_table_to_data_print_str]
     *  CALL %G //ret in %A
     *  POP %A
     *  ret
    */

    Value param0 = func.get_param(0);
    Value ret = func.create_value(Type::void_ptr);

    func.allocate(func, param0, ret);

    Value args_count = func.args_count();
    Value counter = func.create_value(Type::int32);
    func.store(counter, func.create_constant(Type::int32, 0));
    
    Label for_end = func.create_label();
    Label for_begin = func.create_label();
    func.link(for_begin);

    func.if_greater_equals(for_end, counter, args_count);

    Value target_ptr = func.create_value(Type::char_ptr);
    func.store(target, ret);
    func.add(target, counter);
    
    Value source_ptr = func.create_value(Type::char_ptr);
    func.store(source, func.get_args_ptr_begin());
    func.add(source, counter);

    Value source = func.create_value(Type::uint8);
    func.defer(source, source_ptr);

    func.store_deferred(target_ptr, source);

    func.increment(counter);
    func.jump(for_begin);

    func.link(for_end);

    Value print_function = func.create_value(Type::func_ptr);
    auto symbol_constant_name = func.create_constant(Type::char_ptr, "print"); // hmmm...
    func.retrieve_symbol(print_function, symbol_constant_name);
    
    Arguments args {
        func.create_constant(Type::char_ptr, "allocated");
    };

    Value res = func.create_value(Type::int32);
    func.call(print_function, res, args); // func.call(print_function, args); if no return value

    func.retrn(ret);

    ctx.compile();
    ctx.dump(std::cout);

    return 0;
}