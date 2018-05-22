#include <vcrate/code-generator/core/Function.hpp>

#include <vcrate/code-generator/core/Context.hpp>
#include <vcrate/code-generator/core/Block.hpp>
#include <vcrate/code-generator/helper/Dumper.hpp>
#include <vcrate/code-generator/instruction/Encoder.hpp>
#include <vcrate/bytecode/v1.hpp>

#include <stack>
#include <vector>
#include <unordered_set>
#include <unordered_map>

namespace vcrate { namespace code_gen {

Function::Function(Context& context) : context(&context), origin(context.create_block()) {}

Block& Function::get_block() {
    return origin;
}

Value Function::get_parameter(ui32 pos) const {
    return context->create_value(type::none); 
}

}}