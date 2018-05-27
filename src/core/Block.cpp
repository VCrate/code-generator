#include <vcrate/code-generator/core/Block.hpp>

#include <vcrate/code-generator/core/Context.hpp>

namespace vcrate { namespace code_gen {

Block::Block(Context& context) : context(&context) {}

void Block::dump(std::ostream& os) const {
    os << Dumper::bolder() << "Block <" << name << ">" << Dumper::clearer();
    os << " with " << insn.size() << " instructions and terminating by ";
    std::visit(overloaded {
        [&] (Block::EndValueJump const&   v) { os << "a jump to " << v.next->name << '\n'; },
        [&] (Block::EndValueHalt const&    ) { os << "halting\n"; },
        [&] (Block::EndValueThrow const&  v) { os << "a throwing " << v.threw->name << '\n'; },
        [&] (Block::EndValueReturn const& v) { 
            os << "returning ";
            if (v.output)
                os << (*v.output)->name << '\n';
            else
                os << "from the function\n";
        },
        [&] (Block::EndValueBranch const& v) {
            os << "comparing ";
            switch(v.cond) {
                case BranchMethod::Equal: os << "equality"; break;
                case BranchMethod::Different: os << "non equality"; break;
                case BranchMethod::Greater: os << "greatest"; break;
                case BranchMethod::GreaterEqual: os << "non less"; break;
                case BranchMethod::Less: os << "less"; break;
                case BranchMethod::LessEqual: os << "non greatest"; break;
            }
            os << " to " << v.then->name << " or else " << v.otherwise->name << '\n';
        }
    }, end_value);
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

void Block::end_with_throw(Value& v) {
    end_value = Block::EndValueThrow{ &v }; 
    // TODO: set throw value
    // actual version will halt
}

void Block::end_with_halt(Value& v) {
    end_value = Block::EndValueHalt{ &v }; 
}

void Block::end_with_return(Value& v) {
    end_value = Block::EndValueReturn{ { &v } }; 
    // TODO: set return value
}

void Block::end_with_return() {
    end_value = Block::EndValueReturn{ std::nullopt }; 
    // TODO: set return value
}

void Block::insn_load(Value& to, std::vector<ui8> const& raw) {
    insn.push_back({
        Block::InsnOp::Load,
        {&to},
        raw
    });
}

void Block::insn_copy(Value& to, Value& from) {
    insn.push_back({
        Block::InsnOp::Copy,
        {&to, &from},
        {}
    });
}

void Block::insn_dbg(Value& value) {
    insn.push_back({
        Block::InsnOp::Debug,
        {&value},
        {}
    });
}

void Block::insn_compare(Value& lhs, Value& rhs) {
    insn.push_back({
        Block::InsnOp::Compare,
        {&lhs, &rhs},
        {}
    });
}

}}