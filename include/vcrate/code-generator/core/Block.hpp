#pragma once

#include <vcrate/Alias.hpp>
#include <vcrate/code-generator/value/Value.hpp>
#include <vcrate/bytecode/Operations.hpp>
#include <string>
#include <variant>

namespace vcrate { namespace code_gen {

// for std::variant
// std::visit(overloaded {
//     [] (int) {},
//     [] (double) {} 
//     ...
// }, variant)
template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

class Block {
public:

    Value create_value(Type const& type/*, Constant const& constant*/);

    void end_with_jump(Block& block);
    void end_with_branch_eq(Block& then, Block& otherwise);
    void end_with_branch_not_eq(Block& then, Block& otherwise);
    void end_with_branch_gt_eq(Block& then, Block& otherwise);
    void end_with_branch_gt(Block& then, Block& otherwise);
    void end_with_branch_lt_eq(Block& then, Block& otherwise);
    void end_with_branch_lt(Block& then, Block& otherwise);
    void end_with_throw(Value const& v);
    void end_with_halt(Value const& v);
    void end_with_return(Value const& v);

    void insn_load(Value const& to, std::vector<ui8> const& raw);
    void insn_copy(Value const& to, Value const& from);
    void insn_dbg(Value const& value);
    void insn_compare(Value const& lhs, Value const& rhs);
    Value insn_new(Value const& size);

    enum class BranchMethod {
        Equal, Different,
        Greater, GreaterEqual,
        Less, LessEqual
    };

    struct EndValueJump {
        Block* next;
    };

    struct EndValueBranch {
        Block* then;
        Block* otherwise;
        BranchMethod cond;
    };

    struct EndValueHalt {
        Value halt_code;
    };

    struct EndValueReturn {
        Value output;
    };

    struct EndValueThrow {
        Value trew;
    };

    using EndValue = std::variant<EndValueJump, EndValueBranch, EndValueHalt, EndValueReturn, EndValueThrow>;

    enum class InsnOp {
        Copy, Load, 
        Debug
    };

    struct insn_t {
        InsnOp op;
        std::vector<Value> values;
        std::vector<ui8> raw;
    };

    std::string name;
    EndValue end_value;    
    ui32 current_value_id = 0;
    std::vector<insn_t> insn;

};

}}