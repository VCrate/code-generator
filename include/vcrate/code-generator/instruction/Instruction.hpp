#pragma once

#include <vcrate/Alias.hpp>

#include <vcrate/code-generator/helper/Dumper.hpp>
#include <vcrate/code-generator/value/Value.hpp>
#include <vcrate/code-generator/instruction/Operation.hpp>

#include <vector>
#include <initializer_list>

namespace vcrate { namespace code_gen {

class Instruction {
public:

    Instruction(Operation const& ope, std::initializer_list<Value> const& values);

    Operation get_operation() const;
    ui32 get_args_count() const;
    Value get_arg(ui32 index) const;

    std::ostream& dump(std::ostream& os) const;

private:

    Operation ope;
    std::vector<Value> values;

};

}}