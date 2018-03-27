#pragma once

#include <vcrate/Alias.hpp>
#include <vcrate/code-generator/function/Signature.hpp>
#include <vcrate/code-generator/value/Value.hpp>
#include <vcrate/code-generator/value/Constant.hpp>
#include <vcrate/code-generator/instruction/Instruction.hpp>

#include <optional>

namespace vcrate { namespace code_gen {

class Context;

class Function {
public:

    Function(Context& ctx, std::string const& name, Signature const& signature);

    Constant& create_constant(Type const& type, Constant::values_t const& value);
    Value create_value(Type const& type);

    void dump(std::ostream& os) const;

    void retrn(Value const& value);

private:

    Context* ctx;
    std::string name;
    Signature signature;

    std::vector<Instruction> instructions;


    ui32 last_id = 0;

};

}}