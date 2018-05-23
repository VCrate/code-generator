#include <vcrate/code-generator/instruction/Encoder.hpp>

#include <vcrate/bytecode/Operations.hpp>
#include <vcrate/bytecode/v1.hpp>

namespace vcrate { namespace code_gen {

using namespace bytecode;

instruction_t make_binary(Operations ope, Operand const& lhs, Operand const& rhs) {
    using namespace bytecode::v1;
    auto l = lhs.to_12();
    auto r = rhs.to_12();
    return {
        instruction.encode(static_cast<ui32>(ope)) 
        | arg_12a.encode(l.first) | arg_12b.encode(r.first),
        l.second == std::nullopt ? r.second : l.second,
        l.second == std::nullopt ? std::nullopt : r.second
    };
}

instruction_t make_unary(Operations ope, Operand const& lhs) {
    using namespace bytecode::v1;
    return {
        instruction.encode(static_cast<ui32>(ope)) | lhs.to_24().first,
        lhs.to_24().second
    };
}

instruction_t make_simple(Operations ope) {
    using namespace bytecode::v1;
    return {
        instruction.encode(static_cast<ui32>(ope))
    };
}

instruction_t make_add(Operand const& lhs, Operand const& rhs) {
    return make_binary(Operations::ADD, lhs, rhs);
}

instruction_t make_sub(Operand const& lhs, Operand const& rhs) {
    return make_binary(Operations::SUB, lhs, rhs);
}

instruction_t make_mod(Operand const& lhs, Operand const& rhs) {
    return make_binary(Operations::MOD, lhs, rhs);
}

instruction_t make_mul(Operand const& lhs, Operand const& rhs) {
    return make_binary(Operations::MUL, lhs, rhs);
}

instruction_t make_mulu(Operand const& lhs, Operand const& rhs) {
    return make_binary(Operations::MULU, lhs, rhs);
}

instruction_t make_div(Operand const& lhs, Operand const& rhs) {
    return make_binary(Operations::DIV, lhs, rhs);
}

instruction_t make_divu(Operand const& lhs, Operand const& rhs) {
    return make_binary(Operations::DIVU, lhs, rhs);
}

instruction_t make_mov(Operand const& lhs, Operand const& rhs) {
    return make_binary(Operations::MOV, lhs, rhs);
}

instruction_t make_lea(Operand const& lhs, Operand const& rhs) {
    return make_binary(Operations::LEA, lhs, rhs);
}

instruction_t make_pop(Operand const& lhs) {
    return make_unary(Operations::POP, lhs);
}

instruction_t make_push(Operand const& lhs) {
    return make_unary(Operations::PUSH, lhs);
}

instruction_t make_jmp(Operand const& lhs) {
    return make_unary(Operations::JMP, lhs);
}

instruction_t make_jmpe(Operand const& lhs) {
    return make_unary(Operations::JMPE, lhs);
}

instruction_t make_jmpne(Operand const& lhs) {
    return make_unary(Operations::JMPNE, lhs);
}

instruction_t make_jmpg(Operand const& lhs) {
    return make_unary(Operations::JMPG, lhs);
}

instruction_t make_jmpge(Operand const& lhs) {
    return make_unary(Operations::JMPGE, lhs);
}

instruction_t make_and(Operand const& lhs, Operand const& rhs) {
    return make_binary(Operations::AND, lhs, rhs);
}

instruction_t make_or(Operand const& lhs, Operand const& rhs) {
    return make_binary(Operations::OR, lhs, rhs);
}

instruction_t make_xor(Operand const& lhs, Operand const& rhs) {
    return make_binary(Operations::XOR, lhs, rhs);
}

instruction_t make_neg(Operand const& lhs) {
    return make_unary(Operations::NOT, lhs);
}

instruction_t make_shl(Operand const& lhs, Operand const& rhs) {
    return make_binary(Operations::SHL, lhs, rhs);
}

instruction_t make_rtl(Operand const& lhs, Operand const& rhs) {
    return make_binary(Operations::RTL, lhs, rhs);
}

instruction_t make_shr(Operand const& lhs, Operand const& rhs) {
    return make_binary(Operations::SHR, lhs, rhs);
}

instruction_t make_rtr(Operand const& lhs, Operand const& rhs) {
    return make_binary(Operations::RTR, lhs, rhs);
}

instruction_t make_swp(Operand const& lhs, Operand const& rhs) {
    return make_binary(Operations::SWP, lhs, rhs);
}

instruction_t make_cmp(Operand const& lhs, Operand const& rhs) {
    return make_binary(Operations::CMP, lhs, rhs);
}

instruction_t make_cpmu(Operand const& lhs, Operand const& rhs) {
    return make_binary(Operations::CMPU, lhs, rhs);
}

instruction_t make_inc(Operand const& lhs) {
    return make_unary(Operations::INC, lhs);
}

instruction_t make_dec(Operand const& lhs) {
    return make_unary(Operations::DEC, lhs);
}

instruction_t make_new(Operand const& lhs, Operand const& rhs) {
    return make_binary(Operations::NEW, lhs, rhs);
}

instruction_t make_del(Operand const& lhs) {
    return make_unary(Operations::DEL, lhs);
}

instruction_t make_call(Operand const& lhs) {
    return make_unary(Operations::CALL, lhs);
}

instruction_t make_ret() {
    return make_simple(Operations::RET);
}

instruction_t make_etr() {
    return make_simple(Operations::ETR);
}

instruction_t make_lve() {
    return make_simple(Operations::LVE);
}

instruction_t make_hlt() {
    return make_simple(Operations::HLT);
}

instruction_t make_out(Operand const& lhs) {
    return make_unary(Operations::OUT, lhs);
}

instruction_t make_dbg(Operand const& lhs) {
    return make_unary(Operations::DBG, lhs);
}


}}