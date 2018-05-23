#pragma once

#include <vcrate/Alias.hpp>
#include <vcrate/code-generator/instruction/Operand.hpp>

namespace vcrate { namespace code_gen {

struct instruction_t {
    ui32 instruction;
    std::optional<ui32> extra_0 = std::nullopt;
    std::optional<ui32> extra_1 = std::nullopt;
};

instruction_t make_add(Operand const& lhs, Operand const& rhs);
instruction_t make_sub(Operand const& lhs, Operand const& rhs);
instruction_t make_mod(Operand const& lhs, Operand const& rhs);
instruction_t make_mul(Operand const& lhs, Operand const& rhs);
instruction_t make_mulu(Operand const& lhs, Operand const& rhs);
instruction_t make_div(Operand const& lhs, Operand const& rhs);
instruction_t make_divu(Operand const& lhs, Operand const& rhs);
instruction_t make_mov(Operand const& lhs, Operand const& rhs);
instruction_t make_lea(Operand const& lhs, Operand const& rhs);
instruction_t make_pop(Operand const& lhs);
instruction_t make_push(Operand const& lhs);
instruction_t make_jmp(Operand const& lhs);
instruction_t make_jmpe(Operand const& lhs);
instruction_t make_jmpne(Operand const& lhs);
instruction_t make_jmpg(Operand const& lhs);
instruction_t make_jmpge(Operand const& lhs);
instruction_t make_and(Operand const& lhs, Operand const& rhs);
instruction_t make_or(Operand const& lhs, Operand const& rhs);
instruction_t make_xor(Operand const& lhs, Operand const& rhs);
instruction_t make_neg(Operand const& lhs);
instruction_t make_shl(Operand const& lhs, Operand const& rhs);
instruction_t make_rtl(Operand const& lhs, Operand const& rhs);
instruction_t make_shr(Operand const& lhs, Operand const& rhs);
instruction_t make_rtr(Operand const& lhs, Operand const& rhs);
instruction_t make_swp(Operand const& lhs, Operand const& rhs);
instruction_t make_cmp(Operand const& lhs, Operand const& rhs);
instruction_t make_cpmu(Operand const& lhs, Operand const& rhs);
instruction_t make_inc(Operand const& lhs);
instruction_t make_dec(Operand const& lhs);
instruction_t make_new(Operand const& lhs, Operand const& rhs);
instruction_t make_del(Operand const& lhs);
instruction_t make_call(Operand const& lhs);
instruction_t make_ret();
instruction_t make_etr();
instruction_t make_lve();
instruction_t make_hlt();
instruction_t make_out(Operand const& lhs);
instruction_t make_dbg(Operand const& lhs);

}}