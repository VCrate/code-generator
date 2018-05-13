#include <vcrate/code-generator/core/Block.hpp>

namespace vcrate { namespace code_gen {

Value Block::create_value(Type const& type) {
    return Value { current_value_id++, type };
}

void Block::end_with_jump(Block& block) {
    end_value = Block::EndValueJump { &block };
}

void Block::end_with_branch_eq(Block& then, Block& otherwise) {
    end_value = Block::EndValueBranch{
        &then,
        &otherwise,
        Block::BranchMethod::Equal
    };
}

void Block::end_with_branch_not_eq(Block& then, Block& otherwise) {
    end_value = Block::EndValueBranch{
        &then,
        &otherwise,
        Block::BranchMethod::Different
    };
}

void Block::end_with_branch_gt_eq(Block& then, Block& otherwise) {
    end_value = Block::EndValueBranch{
        &then,
        &otherwise,
        Block::BranchMethod::GreaterEqual
    };
}

void Block::end_with_branch_gt(Block& then, Block& otherwise) {
    end_value = Block::EndValueBranch{
        &then,
        &otherwise,
        Block::BranchMethod::Greater
    };
}

void Block::end_with_branch_lt(Block& then, Block& otherwise) {
    end_value = Block::EndValueBranch{
        &then,
        &otherwise,
        Block::BranchMethod::Less
    };
}

void Block::end_with_branch_lt_eq(Block& then, Block& otherwise) {
    end_value = Block::EndValueBranch{
        &then,
        &otherwise,
        Block::BranchMethod::LessEqual
    };
}

void Block::end_with_throw(Value const& v) {
    end_value = Block::EndValueThrow{ v }; 
    // TODO: set throw value
    // actual version will halt
}

void Block::end_with_halt(Value const& v) {
    end_value = Block::EndValueHalt{ v }; 
}

void Block::end_with_return(Value const& v){
    end_value = Block::EndValueReturn{ v }; 
    // TODO: set return value
}

void Block::insn_load(Value const& to, std::vector<ui8> const& raw) {
    insn.push_back({
        Block::InsnOp::Load,
        {to},
        raw
    });
}

void Block::insn_copy(Value const& to, Value const& from) {
    insn.push_back({
        Block::InsnOp::Copy,
        {to, from},
        {}
    });
}

Value Block::insn_new(Value const& size) {
    // TODO
    return create_value(type::ptr);
}

void Block::insn_compare(Value const& lhs, Value const& rhs) {
    // TODO
}

}}