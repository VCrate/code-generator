#include <iostream>

#include <vcrate/Alias.hpp>

int main() {
    using namespace vcrate;
    using namespace vcrate::code_gen;

    Context ctx;

    Signature func_signature;
    // ui32, ui8... -> void*
    func_signature.set_return_type(Type::ptr);
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

    Locale param0 = func.get_param(0);
    Locale ret = func.create_locale(Type::ptr);
    
    func.allocate(func, param0, ret);

    Locale args_count = func.args_count();
    Locale counter = func.create_locale(Type::integer32);
    func.store(counter, func.create_constant(Type::integer32, 0));
    
    Label for_end = func.create_label();
    Label for_begin = func.create_label();
    func.link(for_begin);

    func.if_greater_equals(for_end, counter, args_count);

    Locale target_ptr = func.create_locale(Type::ptr);
    func.store(target, ret);
    func.add(target, counter);
    
    Locale source_ptr = func.create_locale(Type::ptr);
    func.store(source, func.get_args_ptr_begin());
    func.add(source, counter);

    Locale source = func.create_locale(Type::uinteger8);
    func.defer(source, source_ptr);

    func.store_deferred(target_ptr, source);

    func.increment(counter);
    func.jump(for_begin);

    func.link(for_end);

    Locale print_function = func.create_locale(Type::ptr);
    auto symbol_constant_name = func.create_constant(Type::ptr, "print"); // hmmm...
    func.retrieve_symbol(print_function, symbol_constant_name);
    
    Arguments args {
        func.create_constant(Type::ptr, "allocated");
    };

    Locale res = func.create_locale(Type::integer32);
    func.call(print_function, res, args); // func.call(print_function, args); if no return value

    func.retrn(ret);

    return 0;
}