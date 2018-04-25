#include <vcrate/code-generator/core/Block.hpp>

namespace vcrate { namespace code_gen {

Value Block::create_value(Type const& type) {
    // TODO
    return Value { 0, type };
}

void Block::end_with_jump(Block& block) {
    end_method = Block::EndMethod::Jump;
    end_value.next = &block;
}

void Block::end_with_branch(Value const& v, Block& then, Block& otherwise) {
    end_method = Block::EndMethod::Branch;
    end_value.then = &then;
    end_value.otherwise = &otherwise;
}

void Block::end_with_throw(Value const& v) {
    end_method = Block::EndMethod::Throw;
    // TODO: set throw value
    // actual version will print the value, then halt
}

void Block::end_with_halt(Value const& v) {
    end_method = Block::EndMethod::Halt;
}

void Block::end_with_return(Value const& v){
    end_method = Block::EndMethod::Return;
    // TODO: set return value
}

void Block::insn_store(Value const& dest, i32 value) {
    // TODO
}

Value Block::insn_new(Value const& size) {
    // TODO
    return Value { 0, type::ptr };
}

Value Block::insn_inf_eq(Value const& lhs, Value const& rhs) {
    // TODO
    return Value { 0, type::boolean };
}

}}