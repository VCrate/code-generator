#include <vcrate/code-generator/instruction/Instruction.hpp>

#include <algorithm>

namespace vcrate { namespace code_gen {

Instruction::Instruction(Operation const& ope, std::initializer_list<Value> const& values) 
    : ope(ope), values(values) {}

Operation Instruction::get_operation() const {
    return ope;
}

ui32 Instruction::get_args_count() const {
    return values.size();
}

Value Instruction::get_arg(ui32 index) const {
    return values[index];
}

std::ostream& Instruction::dump(std::ostream& os) const {
    std::string is = ope.format;
    for(ui32 i = 0; i < ope.args_count; ++i) {
        std::string to_find = "$" + std::to_string(i); 
        for(auto it = is.find(to_find); it != std::string::npos; it = is.find(to_find))
            is.replace(it, to_find.size(), std::string(values[i].get_type().name));
    }
    return os << is;
}
    
}}