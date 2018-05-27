#pragma once

#include <vcrate/Alias.hpp>
#include <vcrate/code-generator/value/Value.hpp>
#include <vcrate/bytecode/Operations.hpp>
#include <vcrate/code-generator/helper/Dumper.hpp>

#include <string>
#include <variant>
#include <optional>

namespace vcrate { namespace code_gen {

// for std::variant
// std::visit(overloaded {
//     [] (int) {},
//     [] (double) {} 
//     ...
// }, variant)
template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

class Context;

class Block {
public:

    void dump(std::ostream& os = std::cout) const;

    void end_with_jump(Block& block);
    void end_with_branch_eq(Block& then, Block& otherwise);
    void end_with_branch_not_eq(Block& then, Block& otherwise);
    void end_with_branch_gt_eq(Block& then, Block& otherwise);
    void end_with_branch_gt(Block& then, Block& otherwise);
    void end_with_branch_lt_eq(Block& then, Block& otherwise);
    void end_with_branch_lt(Block& then, Block& otherwise);
    void end_with_throw(Value& v);
    void end_with_halt(Value& v);
    void end_with_return(Value& v);
    void end_with_return();

    void insn_load(Value& to, std::vector<ui8> const& raw);
    void insn_copy(Value& to, Value& from);
    void insn_dbg(Value& value);
    void insn_compare(Value& lhs, Value& rhs);

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
        Value* halt_code;
    };

    struct EndValueReturn {
        std::optional<Value*> output;
    };

    struct EndValueThrow {
        Value* threw;
    };

    using EndValue = std::variant<EndValueJump, EndValueBranch, EndValueHalt, EndValueReturn, EndValueThrow>;

    enum class InsnOp {
        Copy, Load, 
        Debug,
        Compare
    };

    struct insn_t {
        InsnOp op;
        std::vector<Value*> values;
        std::vector<ui8> raw;
    };

    std::string name;

private:

    friend class Context;

    Block(Context& context);

    Context* context;
    EndValue end_value;    
    ui32 current_value_id = 0;
    std::vector<insn_t> insn;

};

}}