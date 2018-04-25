#pragma once

#include <vcrate/Alias.hpp>
#include <vcrate/code-generator/value/Value.hpp>
#include <string>

namespace vcrate { namespace code_gen {

class Block {
public:

    Value create_value(Type const& type);

    void end_with_jump(Block& block);
    void end_with_branch(Value const& v, Block& then, Block& otherwise);
    void end_with_throw(Value const& v);
    void end_with_halt(Value const& v);
    void end_with_return(Value const& v);

    void insn_store(Value const& dest, i32 value);
    Value insn_new(Value const& size);
    Value insn_inf_eq(Value const& lhs, Value const& rhs);

    std::string name;

private:

    friend class Function;

    enum class EndMethod {
        Return, Throw, Branch, Halt, Jump
    };

    union EndValue {
        struct {
            Block* next;
        };
        struct {
            Block* then;
            Block* otherwise;
        };
    };

    EndMethod end_method;
    EndValue end_value;

};

}}